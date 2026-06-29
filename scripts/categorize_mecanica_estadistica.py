#!/usr/bin/env python3
"""Genera categorias.json para 'Mecanica Estadistica' a partir de contenido real.

Las categorias se asignaron leyendo un extracto del texto de cada PDF
(ver scripts/extract_pdf_excerpts.py), no solo el nombre de carpeta, porque
varias carpetas (Curso 115A, Ensembles, Material Online) mezclan temas.
Guarda el resultado en assets/categorias_overrides.json, que el indexador
(build_material_index.py) fusiona con el indice por ruta de archivo.
"""
import json
import os
import re

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CURSO = "4y1s - Mecanica Estadistica"

REGLAS = [
    (r"Estrella neutrones/", "Astrofisica"),
    (r"Pruebas pasadas/|Prueba \d ejercicios/", "Pruebas y examenes"),
    (r"Sears Zemansky/", "Libro de texto"),
    (r"Curso 115A/Tareas/", "Tareas y ejercicios"),
    (r"Curso 115A/(IntroSuperconductivity|Lecture11_superconductivity|BoseEinstein_superfluidity)", "Superconductividad y superfluidez"),
    (r"Curso 115A/(PhaseTransitions|Lecture1[89]|Lecture2[0-4])", "Transiciones de fase"),
    (r"Curso 115A/", "Termodinamica"),
    (r"Ensembles/(1_|2_|3_|4_|5_)", "Termodinamica"),
    (r"Ensembles/6_Phase_transitions", "Transiciones de fase"),
    (r"Ensembles/(7_|8_Microcanonical|9_Canonical|10_Grand)", "Ensembles estadisticos"),
    (r"Ensembles/(11_|12_|13_|14_)", "Gases cuanticos"),
    (r"Ensembles/15_Landau", "Transiciones de fase"),
    (r"Ensembles/16_SOC", "Criticidad y sistemas complejos"),
    (r"Material Online/Notas-TermoII-2010-(1|2|3)\.pdf", "Probabilidad"),
    (r"Material Online/Notas-TermoII-2009-1\.pdf|Material Online/Notas-TermoII-2010-4\.pdf", "Mecanica estadistica - fundamentos"),
    (r"Material Online/Notas-TermoII-2009-[234]\.pdf|Material Online/Notas-TermoII-2010-[567]\.pdf", "Ensembles estadisticos"),
    (r"Material Online/Notas-TermoII-2009-[56]\.pdf|Material Online/Notas-TermoII-2010-[89]\.pdf", "Gases cuanticos"),
    (r"Material Online/Notas-TermoII-(2009-[78]|2010-1[012])\.pdf", "Sistemas magneticos"),
    (r"1_Termodinamica/", "Termodinamica"),
    (r"Chuleta_Formulas", "Termodinamica"),
    (r"problemas-y-ejercicios-resueltos-de-termodinamica", "Tareas y ejercicios"),
    (r"crash_course_statistical_mechanics|statistical_mechanics\.pdf$", "Mecanica estadistica - fundamentos"),
    (r"Solved problems in quantum and statistical mechanics", "Tareas y ejercicios"),
    (r"Material Online/(bose|fermi|cuerpo_negro|pauli)\.pdf", "Gases cuanticos"),
    (r"Material Online/gran_canonico\.pdf", "Ensembles estadisticos"),
    (r"Material Online/(ising_varios|isingd1|isingd2|magnetismo1|magnetismo2|campo_medio)", "Sistemas magneticos"),
]


def categoria_para(ruta):
    for patron, categoria in REGLAS:
        if re.search(patron, ruta):
            return categoria
    return None


def main():
    material = json.load(open(os.path.join(REPO_ROOT, "assets", "material.json"), encoding="utf-8"))
    overrides_path = os.path.join(REPO_ROOT, "assets", "categorias_overrides.json")
    overrides = {}
    if os.path.exists(overrides_path):
        overrides = json.load(open(overrides_path, encoding="utf-8"))

    asignadas = 0
    sin_regla = []
    for item in material["items"]:
        if item["curso"] != CURSO:
            continue
        categoria = categoria_para(item["ruta"])
        if categoria:
            overrides[item["ruta"]] = categoria
            asignadas += 1
        else:
            sin_regla.append(item["ruta"])

    with open(overrides_path, "w", encoding="utf-8") as f:
        json.dump(overrides, f, ensure_ascii=False, indent=0, sort_keys=True)

    print(f"Asignadas {asignadas} categorias para '{CURSO}'.")
    if sin_regla:
        print(f"Sin regla ({len(sin_regla)}):")
        for r in sin_regla:
            print(" -", r)


if __name__ == "__main__":
    main()
