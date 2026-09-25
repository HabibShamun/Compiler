"""A small desktop editor for the Bangla compiler."""

from __future__ import annotations

from pathlib import Path
import shutil
import subprocess
import tempfile
import tkinter as tk
import tkinter.font as tkfont
from tkinter import filedialog, messagebox, ttk


ROOT = Path(__file__).resolve().parent
SOURCES = [
    "main.cpp",
    "Lexer.cpp",
    "parser.cpp",
    "PrettyPrinter.cpp",
    "SemanticAnalyzer.cpp",
    "Token.cpp",
    "PythonGenerator.cpp",
    "TAC.cpp",
]
PRACTICE_PROGRAMS = [
    ("Hello, Bangla!", "Print a Bangla greeting.", '''dekhao "আসসালামু আলাইকুম"
'''),
    ("Variables", "Declare integer and text variables, then print them.", '''shonkha boyos = 18
lekha naam = "Rafi"

dekhao naam
dekhao boyos
'''),
    ("Math", "Try changing the numbers or arithmetic operators.", '''shonkha first = 12
shonkha second = 5

dekhao first + second
dekhao first * second
'''),
    ("If / else", "Change the age or comparison and run it again.", '''shonkha boyos = 16

jodi boyos >= 18 {
    dekhao "You are an adult"
} nahole {
    dekhao "You are under 18"
}
'''),
    ("While loop", "The loop counts from 1 to 5. Try changing its limit.", '''shonkha count = 1

jotokhon count <= 5 {
    dekhao count
    count = count + 1
}
'''),
    ("বাংলা keywords", "বাংলা keywords ও অঙ্ক দিয়ে একটি program লিখুন।", '''সংখ্যা বয়স = ১৮
লেখা নাম = "রাফি"

দেখাও নাম
দেখাও বয়স
'''),
]


def find_gpp() -> str | None:
    return shutil.which("g++") or shutil.which("g++.exe")


class BanglaIDE(tk.Tk):
    def __init__(self) -> None:
        super().__init__()
        self.title("Bhasha — Bangla Programming Practice")
        self.geometry("1100x760")
        self.minsize(760, 520)
        self.current_file: Path | None = None
        self.selected_lesson = 0
        self.compiler_path = ROOT / "compiler_ide.exe"
        self._make_widgets()
        self.lesson_list.selection_set(0)
        self._show_lesson(0)

    def _make_widgets(self) -> None:
        self.configure(bg="#10151d")
        style = ttk.Style(self)
        style.theme_use("clam")
        style.configure("TFrame", background="#10151d")
        style.configure("Toolbar.TFrame", background="#171e29")
        style.configure("TLabel", background="#10151d", foreground="#dbe5f2")
        style.configure("Title.TLabel", background="#171e29", foreground="#ffffff", font=("Segoe UI", 14, "bold"))
        style.configure("Hint.TLabel", background="#171e29", foreground="#9aa9bc", font=("Segoe UI", 9))
        style.configure("TButton", font=("Segoe UI", 10), padding=(12, 7))
        style.configure("Run.TButton", background="#19a974", foreground="#ffffff", font=("Segoe UI", 10, "bold"))
        style.map("Run.TButton", background=[("active", "#13875d")])
        available_fonts = {family.casefold() for family in tkfont.families(self)}
        source_font = "Nirmala UI" if "nirmala ui" in available_fonts else "Segoe UI"

        toolbar = ttk.Frame(self, style="Toolbar.TFrame", padding=(16, 12))
        toolbar.pack(fill="x")
        ttk.Label(toolbar, text="Bhasha", style="Title.TLabel").pack(side="left")
        ttk.Label(toolbar, text="Bangla programming practice", style="Hint.TLabel").pack(side="left", padx=(14, 0))
        ttk.Button(toolbar, text="New", command=self.new_file).pack(side="right", padx=(8, 0))
        ttk.Button(toolbar, text="Open", command=self.open_file).pack(side="right", padx=(8, 0))
        ttk.Button(toolbar, text="Save", command=self.save_file).pack(side="right", padx=(8, 0))
        ttk.Button(toolbar, text="Reset example", command=self.reset_lesson).pack(side="right", padx=(8, 0))
        ttk.Button(toolbar, text="▶  Run", style="Run.TButton", command=self.run_program).pack(side="right")

        self.panes = ttk.Panedwindow(self, orient="vertical")
        self.panes.pack(fill="both", expand=True, padx=12, pady=(12, 8))

        workspace = ttk.Panedwindow(self.panes, orient="horizontal")
        self.panes.add(workspace, weight=4)

        lessons_panel = ttk.Frame(workspace, padding=(4, 0, 8, 0))
        ttk.Label(lessons_panel, text="Practice programs", font=("Segoe UI", 10, "bold")).pack(anchor="w", pady=(0, 6))
        self.lesson_list = tk.Listbox(
            lessons_panel,
            width=19,
            height=12,
            font=("Segoe UI", 10),
            bg="#171e29",
            fg="#dbe5f2",
            selectbackground="#275b80",
            selectforeground="#ffffff",
            activestyle="none",
            relief="flat",
            highlightthickness=0,
            exportselection=False,
        )
        for name, _description, _source in PRACTICE_PROGRAMS:
            self.lesson_list.insert("end", name)
        self.lesson_list.pack(fill="x")
        self.lesson_list.bind("<<ListboxSelect>>", self._select_lesson)
        self.lesson_description = ttk.Label(lessons_panel, text="", wraplength=165, justify="left", foreground="#9aa9bc")
        self.lesson_description.pack(fill="x", anchor="w", pady=(10, 0))
        workspace.add(lessons_panel, weight=1)

        editor_panel = ttk.Frame(workspace)
        editor_header = ttk.Frame(editor_panel, padding=(4, 0, 4, 6))
        editor_header.pack(fill="x")
        self.file_label = ttk.Label(editor_header, text="Untitled.bangla", font=("Segoe UI", 10, "bold"))
        self.file_label.pack(side="left")
        ttk.Label(editor_header, text="Bangla source", foreground="#8d9caf").pack(side="right")
        editor_body = ttk.Frame(editor_panel)
        editor_body.pack(fill="both", expand=True)
        self.editor = tk.Text(
            editor_body,
            wrap="none",
            undo=True,
            font=(source_font, 13),
            bg="#171e29",
            fg="#e6edf6",
            insertbackground="#ffffff",
            selectbackground="#275b80",
            relief="flat",
            padx=12,
            pady=10,
            tabs=(32,),
        )
        self.editor.pack(side="left", fill="both", expand=True)
        editor_scroll = ttk.Scrollbar(editor_body, orient="vertical", command=self.editor.yview)
        editor_scroll.pack(side="right", fill="y")
        self.editor.configure(yscrollcommand=editor_scroll.set)
        self.editor.bind("<<Modified>>", self._editor_changed)
        workspace.add(editor_panel, weight=4)

        reference_panel = ttk.Frame(workspace, padding=(8, 0, 2, 0))
        ttk.Label(reference_panel, text="Language guide", font=("Segoe UI", 10, "bold")).pack(anchor="w", pady=(0, 6))
        self.reference = tk.Text(
            reference_panel,
            width=25,
            wrap="word",
            font=(source_font, 10),
            bg="#171e29",
            fg="#c7d2e0",
            relief="flat",
            padx=10,
            pady=10,
            state="normal",
        )
        self.reference.pack(fill="both", expand=True)
        self.reference.insert("1.0", "KEYWORDS\n\nshonkha / সংখ্যা\ninteger\n\nlekha / লেখা\ntext\n\ndekhao / দেখাও\nprint\n\njodi / যদি\nif\n\nnahole / নাহলে\nelse\n\njotokhon / যতক্ষণ\nwhile\n\nSYNTAX\n\nname = expression\n\nif condition {\n    statements\n}\n\nUse + - * / for math.\nUse < <= > >= == != to compare.\n\nPrograms use one statement per line. Put text in double quotes.")
        self.reference.configure(state="disabled")
        workspace.add(reference_panel, weight=1)

        output_panel = ttk.Frame(self.panes)
        output_header = ttk.Frame(output_panel, padding=(4, 0, 4, 6))
        output_header.pack(fill="x")
        ttk.Label(output_header, text="Output", font=("Segoe UI", 10, "bold")).pack(side="left")
        self.status = ttk.Label(output_header, text="Ready", foreground="#8d9caf")
        self.status.pack(side="right")
        self.output = tk.Text(
            output_panel,
            height=10,
            wrap="word",
            font=(source_font, 10),
            bg="#0b1017",
            fg="#c7d2e0",
            insertbackground="#ffffff",
            relief="flat",
            padx=12,
            pady=10,
            state="disabled",
        )
        self.output.pack(fill="both", expand=True)
        self.panes.add(output_panel, weight=2)
        ttk.Label(self, text="Ctrl+N  New     Ctrl+S  Save     Ctrl+O  Open     F5  Run", foreground="#8290a3").pack(anchor="e", padx=16, pady=(0, 8))
        self.bind("<Control-n>", lambda _event: self.new_file())
        self.bind("<Control-s>", lambda _event: self.save_file())
        self.bind("<Control-o>", lambda _event: self.open_file())
        self.bind("<F5>", lambda _event: self.run_program())

    def _set_source(self, source: str) -> None:
        self.editor.delete("1.0", "end")
        self.editor.insert("1.0", source)
        self.editor.edit_modified(False)
        self._editor_changed()

    def _editor_changed(self, _event: tk.Event | None = None) -> None:
        dirty = self.editor.edit_modified()
        self.file_label.configure(text=self.document_label + ("  •" if dirty else ""))

    def _show_lesson(self, index: int) -> None:
        name, description, source = PRACTICE_PROGRAMS[index]
        self.selected_lesson = index
        self.current_file = None
        self.document_label = f"Practice: {name}"
        self._set_source(source)
        self.lesson_description.configure(text=description)
        self.status.configure(text="Practice program loaded")

    def reset_lesson(self) -> None:
        if not self._confirm_replace():
            return
        self._show_lesson(self.selected_lesson)

    def _confirm_replace(self) -> bool:
        if not self.editor.edit_modified():
            return True
        save = messagebox.askyesnocancel(
            "Unsaved changes",
            "Save your changes before opening another program?",
            parent=self,
        )
        if save is None:
            return False
        return self.save_file() if save else True

    def _select_lesson(self, _event: tk.Event) -> None:
        selection = self.lesson_list.curselection()
        if not selection:
            return
        index = selection[0]
        if index == self.selected_lesson:
            return
        if not self._confirm_replace():
            self.lesson_list.selection_clear(0, "end")
            self.lesson_list.selection_set(self.selected_lesson)
            return
        self._show_lesson(index)

    def _source(self) -> str:
        return self.editor.get("1.0", "end-1c")

    def _write_output(self, text: str) -> None:
        self.output.configure(state="normal")
        self.output.delete("1.0", "end")
        self.output.insert("1.0", text.rstrip() + "\n")
        self.output.configure(state="disabled")

    def open_file(self) -> None:
        chosen = filedialog.askopenfilename(
            title="Open Bangla source",
            filetypes=[("Bangla source", "*.bangla"), ("Text files", "*.txt"), ("All files", "*.*")],
        )
        if not chosen:
            return
        if not self._confirm_replace():
            return
        path = Path(chosen)
        try:
            self._set_source(path.read_text(encoding="utf-8-sig"))
        except (OSError, UnicodeError) as exc:
            messagebox.showerror("Could not open file", str(exc))
            return
        self.current_file = path
        self.document_label = path.name
        self._editor_changed()
        self.status.configure(text="Opened")

    def new_file(self) -> None:
        if not self._confirm_replace():
            return
        self.current_file = None
        self.document_label = "Untitled.bangla"
        self._set_source("")
        self.status.configure(text="New program")

    def save_file(self) -> bool:
        if self.current_file is None:
            chosen = filedialog.asksaveasfilename(
                title="Save Bangla source",
                defaultextension=".bangla",
                filetypes=[("Bangla source", "*.bangla"), ("All files", "*.*")],
            )
            if not chosen:
                return False
            self.current_file = Path(chosen)
        try:
            self.current_file.write_text(self._source(), encoding="utf-8", newline="\n")
        except OSError as exc:
            messagebox.showerror("Could not save file", str(exc))
            return False
        self.document_label = self.current_file.name
        self.editor.edit_modified(False)
        self._editor_changed()
        self.status.configure(text="Saved")
        return True

    def build_compiler(self) -> bool:
        compiler = find_gpp()
        if not compiler:
            self._write_output("Could not find g++. Install a C++17 compiler and make sure g++ is on PATH.")
            self.status.configure(text="Compiler missing")
            return False
        command = [compiler, "-std=c++17", *SOURCES, "-o", str(self.compiler_path)]
        try:
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
        except OSError as exc:
            self._write_output(f"Could not start g++:\n{exc}")
            self.status.configure(text="Build failed")
            return False
        if result.returncode:
            self._write_output(result.stdout + result.stderr or "Build failed without a compiler message.")
            self.status.configure(text="Build failed")
            return False
        self.status.configure(text="Compiler ready")
        return True

    def _compiler_is_current(self) -> bool:
        if not self.compiler_path.exists():
            return False
        compiler_time = self.compiler_path.stat().st_mtime
        return all((ROOT / source).stat().st_mtime <= compiler_time for source in SOURCES)

    def run_program(self) -> None:
        if not self._compiler_is_current() and not self.build_compiler():
            return
        python = shutil.which("python") or shutil.which("py")
        if not python:
            self._write_output("Could not find Python on PATH.")
            self.status.configure(text="Python missing")
            return
        self.status.configure(text="Running…")
        self.update_idletasks()
        try:
            with tempfile.TemporaryDirectory(prefix="bangla_compiler_") as temp_dir:
                temp_path = Path(temp_dir)
                source_path = temp_path / "program.bangla"
                generated_path = temp_path / "program.py"
                source_path.write_text(self._source(), encoding="utf-8", newline="\n")
                compiled = subprocess.run(
                    [str(self.compiler_path), str(source_path), str(generated_path)],
                    cwd=ROOT,
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=30,
                )
                if compiled.returncode:
                    self._write_output(compiled.stdout + compiled.stderr or "Compilation failed.")
                    self.status.configure(text="Compile error")
                    return
                execution = subprocess.run(
                    [python, "-X", "utf8", str(generated_path)],
                    cwd=ROOT,
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=30,
                )
                result_text = execution.stdout
                if execution.stderr:
                    result_text += ("\n" if result_text else "") + execution.stderr
                self._write_output(result_text or "Program finished with no output.")
                self.status.configure(text="Finished" if execution.returncode == 0 else "Runtime error")
        except subprocess.TimeoutExpired:
            self._write_output("Program stopped after 30 seconds (possible infinite loop).")
            self.status.configure(text="Stopped")
        except OSError as exc:
            self._write_output(f"Could not run the program:\n{exc}")
            self.status.configure(text="Run failed")


if __name__ == "__main__":
    app = BanglaIDE()
    app.mainloop()
