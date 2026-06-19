#!/usr/bin/env python3
"""Genera assets/material.json escaneando el repositorio para el buscador web.

Recorre todos los cursos/carpetas del repo y construye un indice plano de
archivos de estudio (pdf, md, docx, pptx, ipynb, etc). Se ejecuta cada vez
que se agrega material nuevo:

    python3 scripts/build_material_index.py
"""
import json
import os

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Carpetas que no son material de estudio.
EXCLUDE_DIRS = {".git", "node_modules", "assets", "scripts", "__pycache__"}

# Extensiones consideradas "material de estudio".
INCLUDE_EXT = {
    ".pdf", ".md", ".txt", ".docx", ".doc", ".ppt", ".pptx",
    ".ipynb", ".nb", ".htm", ".html", ".srt", ".xlsx",
}

EXCLUDE_FILES = {"desktop.ini"}


def human_kind(ext):
    return {
        ".pdf": "PDF",
        ".md": "Apunte",
        ".txt": "Texto",
        ".docx": "Word",
        ".doc": "Word",
        ".ppt": "Presentacion",
        ".pptx": "Presentacion",
        ".ipynb": "Notebook",
        ".nb": "Mathematica",
        ".htm": "Web",
        ".html": "Web",
        ".srt": "Subtitulos",
        ".xlsx": "Excel",
    }.get(ext, ext.lstrip("."))


def cargar_overrides():
    path = os.path.join(REPO_ROOT, "assets", "categorias_overrides.json")
    if os.path.exists(path):
        with open(path, encoding="utf-8") as f:
            return json.load(f)
    return {}


def build_index(overrides):
    items = []
    for dirpath, dirnames, filenames in os.walk(REPO_ROOT):
        dirnames[:] = [d for d in dirnames if d not in EXCLUDE_DIRS and not d.startswith(".")]
        rel_dir = os.path.relpath(dirpath, REPO_ROOT)
        if rel_dir == ".":
            rel_dir = ""

        top_level = rel_dir.split(os.sep)[0] if rel_dir else None
        subcarpeta = rel_dir.split(os.sep)[1] if os.sep in rel_dir else None

        for filename in filenames:
            if filename in EXCLUDE_FILES:
                continue
            ext = os.path.splitext(filename)[1].lower()
            if ext not in INCLUDE_EXT:
                continue

            rel_path = os.path.join(rel_dir, filename) if rel_dir else filename
            rel_path = rel_path.replace(os.sep, "/")
            categoria = overrides.get(rel_path) or subcarpeta or top_level or "General"
            items.append({
                "curso": top_level or "Raiz",
                "carpeta": rel_dir,
                "categoria": categoria,
                "nombre": os.path.splitext(filename)[0],
                "archivo": filename,
                "tipo": human_kind(ext),
                "ruta": rel_path,
            })

    items.sort(key=lambda i: (i["curso"], i["carpeta"], i["nombre"]))
    return items


def main():
    overrides = cargar_overrides()
    items = build_index(overrides)
    cursos = sorted({i["curso"] for i in items})
    out_path = os.path.join(REPO_ROOT, "assets", "material.json")
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump({"cursos": cursos, "items": items}, f, ensure_ascii=False, indent=0)
    print(f"Generados {len(items)} items de {len(cursos)} cursos en {out_path}")


if __name__ == "__main__":
    main()
