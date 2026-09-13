#!/usr/bin/env python3
"""Genera assets/graph.json a partir de assets/material.json para el modo grafo.

Construye una red de nodos (cursos, categorias, tags y apuntes) y enlaces
(contencion curso->categoria->apunte, y asociacion apunte->tag) que permite
visualizar como se conectan los temas entre si, ademas de los apuntes que
existen. Los nodos de tipo "tag" son los que cruzan cursos y categorias
distintas, revelando conexiones tematicas que la lista plana no muestra.

Se ejecuta despues de build_material_index.py:

    python3 scripts/build_material_index.py
    python3 scripts/build_graph_index.py
"""
import json
import os

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MATERIAL_PATH = os.path.join(REPO_ROOT, "assets", "material.json")
OUT_PATH = os.path.join(REPO_ROOT, "assets", "graph.json")


def curso_id(curso):
    return f"curso:{curso}"


def categoria_id(curso, categoria):
    return f"categoria:{curso}|{categoria}"


def tag_id(tag):
    return f"tag:{tag}"


def doc_id(ruta):
    return f"doc:{ruta}"


def build_graph(material):
    items = material["items"]
    cursos = material["cursos"]

    nodes = {}
    links = []

    def add_node(node_id, **kwargs):
        if node_id not in nodes:
            nodes[node_id] = {"id": node_id, **kwargs}
        return nodes[node_id]

    for curso in cursos:
        add_node(curso_id(curso), tipo="curso", label=curso)

    categorias_vistas = set()
    tags_vistos = set()

    for item in items:
        curso = item["curso"]
        categoria = item["categoria"]
        cid = categoria_id(curso, categoria)
        if cid not in categorias_vistas:
            categorias_vistas.add(cid)
            add_node(cid, tipo="categoria", label=categoria, curso=curso)
            links.append({"source": curso_id(curso), "target": cid, "tipo": "contiene"})

        did = doc_id(item["ruta"])
        add_node(
            did,
            tipo="apunte",
            label=item["nombre"],
            curso=curso,
            categoria=categoria,
            archivo=item["tipo"],
            ruta=item["ruta"],
        )
        links.append({"source": cid, "target": did, "tipo": "contiene"})

        for tag in item.get("tags") or []:
            tid = tag_id(tag)
            if tid not in tags_vistos:
                tags_vistos.add(tid)
                add_node(tid, tipo="tag", label=tag)
            links.append({"source": did, "target": tid, "tipo": "etiqueta"})

    return {
        "nodes": list(nodes.values()),
        "links": links,
    }


def main():
    with open(MATERIAL_PATH, encoding="utf-8") as f:
        material = json.load(f)

    graph = build_graph(material)

    with open(OUT_PATH, "w", encoding="utf-8") as f:
        json.dump(graph, f, ensure_ascii=False, indent=0)

    conteo_tipos = {}
    for n in graph["nodes"]:
        conteo_tipos[n["tipo"]] = conteo_tipos.get(n["tipo"], 0) + 1
    print(f"Generados {len(graph['nodes'])} nodos ({conteo_tipos}) y {len(graph['links'])} enlaces en {OUT_PATH}")


if __name__ == "__main__":
    main()
