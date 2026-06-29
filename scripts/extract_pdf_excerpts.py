#!/usr/bin/env python3
"""Extrae un extracto de texto de cada PDF de un curso para categorizacion.

Uso:
    python3 scripts/extract_pdf_excerpts.py "<curso>" salida.json
"""
import json
import os
import subprocess
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def extraer_texto(ruta_pdf, max_paginas=3, max_chars=2000):
    try:
        salida = subprocess.run(
            ["pdftotext", "-l", str(max_paginas), ruta_pdf, "-"],
            capture_output=True, timeout=30,
        )
    except Exception as e:
        return f"[error abriendo pdf: {e}]"
    texto = salida.stdout.decode("utf-8", errors="ignore")
    texto = " ".join(texto.split())
    return texto[:max_chars]


def main():
    curso = sys.argv[1]
    salida = sys.argv[2]

    material = json.load(open(os.path.join(REPO_ROOT, "assets", "material.json"), encoding="utf-8"))
    items = [i for i in material["items"] if i["curso"] == curso and i["tipo"] == "PDF"]

    resultado = []
    for item in items:
        ruta_abs = os.path.join(REPO_ROOT, item["ruta"])
        excerpt = extraer_texto(ruta_abs)
        resultado.append({"ruta": item["ruta"], "nombre": item["nombre"], "carpeta": item["carpeta"], "extracto": excerpt})
        print(f"OK: {item['ruta']}", file=sys.stderr)

    with open(salida, "w", encoding="utf-8") as f:
        json.dump(resultado, f, ensure_ascii=False, indent=0)
    print(f"Listo: {len(resultado)} extractos en {salida}", file=sys.stderr)


if __name__ == "__main__":
    main()
