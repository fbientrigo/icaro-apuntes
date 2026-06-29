#!/usr/bin/env python3
"""Asigna tags libres (no taxonomia fija) a los items de
'3y2s - Brakets Electivo', basado en lectura de extractos de cada PDF.

Escribe/actualiza assets/tags_overrides.json (ruta -> lista de tags),
que build_material_index.py fusiona como item["tags"].

    python3 scripts/tag_brakets_electivo.py
"""
import json
import os

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CURSO = "3y2s - Brakets Electivo"

# (substring del nombre de archivo, tags)
REGLAS = [
    ("Baby-GODZINTEGRAL", ["problema propuesto", "integrales multivariable", "funciones de Bessel"]),
    ("Gruneisen_Theor.Math.Phys", ["funcion de Bloch-Gruneisen", "fisica del estado solido", "series especiales"]),
    ("Complemento_III", ["formalismo MoB", "series binomiales", "apunte de clase"]),
    ("Complemento_II_-_Funci", ["funcion Gamma", "funciones especiales", "apunte de clase"]),
    ("Complemento_IV", ["formalismo MoB", "apunte de clase"]),
    ("Complemento_I_-_Series_hipergeom", ["series hipergeometricas", "funciones especiales", "apunte de clase"]),
    ("Complemento_V", ["representaciones nulas y divergentes", "formalismo MoB", "apunte de clase"]),
    ("Formulario_II", ["formulario", "funciones hipergeometricas", "funciones especiales"]),
    ("Formulario_III", ["formulario", "formalismo MoB", "reglas y teoremas"]),
    ("BracketSolverLib2", ["codigo Maple", "libreria de calculo", "formalismo MoB"]),
    ("PosterAI_IG", ["poster", "sistemas no lineales", "ecuaciones diferenciales", "transformada de Mellin"]),
    ("PosterDS_IG_1", ["poster", "problema de N-cuerpos", "mecanica celeste", "ecuaciones diferenciales"]),
    ("PosterDS_IG_2", ["poster", "atomo de hidrogeno", "mecanica cuantica", "funciones hipergeometricas"]),
    ("PosterEO_IG", ["poster", "continuacion analitica", "funciones hipergeometricas"]),
    ("PosterMF_IG", ["poster", "ecuacion de Lane-Emden", "astrofisica", "politropos"]),
    ("PosterPC_IG", ["poster", "reglas y teoremas", "formalismo MoB"]),
    ("PosterSB_IG", ["poster", "funcion de Fermi-Dirac", "gas cuantico", "astrofisica"]),
    ("Poster_IG", ["poster", "formalismo MoB", "teorema maestro de Ramanujan"]),
    ("Programa_UV", ["programa del curso", "material administrativo"]),
    ("2007-Optimized_NDIM_and_multiloop", ["publicacion", "diagramas de Feynman", "NDIM", "teoria cuantica de campos"]),
    ("2008-Modular_application_of_an_integration_by_fractional", ["publicacion", "diagramas de Feynman", "integracion por expansion fraccional"]),
    ("2009-Feynman_diagrams_and_a_combination", ["publicacion", "diagramas de Feynman", "integracion por partes"]),
    ("2009-Modular_application_of_an_integration_by_fractional", ["publicacion", "diagramas de Feynman", "integracion por expansion fraccional"]),
    ("2010-Method_of_Brackets_and_Feynman_diagram_evaluation", ["publicacion", "diagramas de Feynman", "formalismo MoB", "teorema maestro de Ramanujan"]),
    ("2014-Evaluation_of_entries_in_Gradshteyn_and_Ryzhik", ["publicacion", "tabla de integrales Gradshteyn-Ryzhik", "formalismo MoB"]),
    ("2015-Ramanuj", ["publicacion", "teorema maestro de Ramanujan", "diagramas de Feynman"]),
    ("2017-The_Method_of_Brackets_in_experimental_mathematics", ["publicacion", "matematica experimental", "formalismo MoB"]),
    ("2017-The_Moments_of_the_Hydrogen_Atom", ["publicacion", "atomo de hidrogeno", "mecanica cuantica", "funciones hipergeometricas"]),
    ("2022-Analytic_Expressions_for_Debye_Functions", ["publicacion", "funcion de Debye", "capacidad calorifica", "fisica del estado solido"]),
    ("2007-Optimized_negative_dimensional_integration_method", ["publicacion", "diagramas de Feynman", "NDIM", "teoria cuantica de campos"]),
    ("2010-Definite_integrals_by_the_method_of_brackets._Part_1", ["publicacion", "formalismo MoB", "articulo fundacional", "reglas y teoremas"]),
    ("2010-The_method_of_brackets._Part_2", ["publicacion", "formalismo MoB", "ejemplos y aplicaciones"]),
    ("2012-Ramanuj", ["publicacion", "teorema maestro de Ramanujan", "funciones hipergeometricas"]),
    ("2016-A_new_rule_for_the_method_of_brackets", ["publicacion", "formalismo MoB", "funciones de Bessel", "simbolo de Pochhammer"]),
    ("2017-An_extension_of_the_method_of_brackets._Part_1", ["publicacion", "formalismo MoB", "series nulas y divergentes", "tabla de integrales Gradshteyn-Ryzhik"]),
    ("2017-Integrals_of_Frullani_type", ["publicacion", "integrales de Frullani", "formalismo MoB"]),
    ("2020-An_extension_of_the_method_of_brackets._Part_2", ["publicacion", "formalismo MoB", "transformada de Mellin", "funciones de Bessel"]),
    ("2022-Mellin_Barnes_Integrals_and_the_Method_of_Brackets", ["publicacion", "integrales de Mellin-Barnes", "formalismo MoB"]),
    ("Alonso_Guerrero", ["abstract SOCHIFI", "series de potencias", "formalismo MoB"]),
    ("Grawe", ["abstract SOCHIFI", "integrales multivariable", "formalismo MoB"]),
    ("Yapur", ["abstract SOCHIFI", "series de potencias", "funciones de Bessel", "formalismo MoB"]),
    ("Dan_Mihai", ["abstract SOCHIFI", "continuacion analitica", "funciones hipergeometricas"]),
    ("Diego_Navia", ["abstract SOCHIFI", "coeficiente del virial", "termodinamica", "funciones hipergeometricas"]),
    ("Ivan_Gonzalez_AbstractSOCHIFI", ["abstract SOCHIFI", "integrales de Mellin-Barnes", "diagramas de Feynman"]),
    ("Complementario_Taller_II", ["taller", "transformada de Mellin", "funciones de Bessel", "funcion integral exponencial"]),
    ("Taller_I", ["taller", "ejercicios", "funcion integral exponencial", "funciones de Bessel"]),
    ("Taller_II", ["taller", "ejercicios", "transformada de Mellin", "funciones de Bessel"]),
    ("Problemas_I", ["problemas resueltos", "material de practica"]),
    ("Problemas_II", ["problemas resueltos", "material de practica"]),
    ("Tarea_I", ["tarea", "ejercicios", "coeficiente del virial", "funciones hipergeometricas"]),
    ("Tarea_II", ["tarea", "ejercicios", "transformada de Mellin"]),
]


def tags_para(archivo):
    for patron, tags in REGLAS:
        if patron in archivo:
            return tags
    return None


def main():
    material_path = os.path.join(REPO_ROOT, "assets", "material.json")
    overrides_path = os.path.join(REPO_ROOT, "assets", "tags_overrides.json")

    with open(material_path, encoding="utf-8") as f:
        material = json.load(f)

    overrides = {}
    if os.path.exists(overrides_path):
        with open(overrides_path, encoding="utf-8") as f:
            overrides = json.load(f)

    asignados = 0
    sin_match = []
    for item in material["items"]:
        if item["curso"] != CURSO:
            continue
        tags = tags_para(item["archivo"])
        if tags:
            overrides[item["ruta"]] = tags
            asignados += 1
        else:
            sin_match.append(item["ruta"])

    overrides = dict(sorted(overrides.items()))
    with open(overrides_path, "w", encoding="utf-8") as f:
        json.dump(overrides, f, ensure_ascii=False, indent=0)

    print(f"Tags asignados a {asignados} items de '{CURSO}'.")
    if sin_match:
        print(f"Sin coincidencia ({len(sin_match)}):")
        for r in sin_match:
            print(f"  {r}")


if __name__ == "__main__":
    main()
