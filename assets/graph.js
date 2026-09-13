(function () {
  // Modulo del "modo grafo". Es independiente de app.js: si este archivo o la
  // libreria force-graph fallan en cargar, la vista de lista sigue funcionando
  // normal. Todo el estado del grafo vive dentro de esta IIFE.

  const elToggle = document.getElementById("toggleVista");
  const elVistaGrafo = document.getElementById("vistaGrafo");
  const elResultados = document.getElementById("resultados");
  const elContador = document.getElementById("contador");
  const elLienzo = document.getElementById("grafoLienzo");
  const elInfo = document.getElementById("grafoInfo");
  const elReset = document.getElementById("grafoReset");
  const elBusqueda = document.getElementById("busqueda");
  const elFiltroCurso = document.getElementById("filtroCurso");
  const checkboxesTipo = Array.from(document.querySelectorAll(".grafo-filtro-tipo"));

  if (!elToggle || !elVistaGrafo) return;

  const COLORES = {
    curso: "#6ea8fe",
    categoria: "#8fd3a4",
    tag: "#f6ad55",
    apunte: "#7a7f95",
  };

  const TAMANOS = {
    curso: 9,
    categoria: 6,
    tag: 6,
    apunte: 2.5,
  };

  let grafoCrudo = null; // { nodes, links } tal cual viene de graph.json
  let instanciaGrafo = null;
  let grafoActivo = false;
  let cargando = false;

  function normalizar(texto) {
    return texto
      .toLowerCase()
      .normalize("NFD")
      .replace(/[̀-ͯ]/g, "");
  }

  function tiposActivos() {
    return new Set(checkboxesTipo.filter((cb) => cb.checked).map((cb) => cb.value));
  }

  function construirSubgrafo() {
    if (!grafoCrudo) return { nodes: [], links: [] };
    const tipos = tiposActivos();
    const cursoFiltro = elFiltroCurso ? elFiltroCurso.value : "";

    let nodos = grafoCrudo.nodes.filter((n) => tipos.has(n.tipo));
    if (cursoFiltro) {
      // Los tags no tienen "curso" propio (son transversales): se deciden
      // aparte, por si conectan con algun apunte que sobrevivio el filtro.
      const directos = nodos.filter((n) => {
        if (n.tipo === "tag") return false;
        if (n.tipo === "curso") return n.label === cursoFiltro;
        return n.curso === cursoFiltro;
      });
      const idsDirectos = new Set(directos.map((n) => n.id));
      const tagsConectados = new Set(
        grafoCrudo.links
          .filter((l) => idsDirectos.has(l.source) || idsDirectos.has(l.target))
          .flatMap((l) => [l.source, l.target])
      );
      nodos = nodos.filter(
        (n) => idsDirectos.has(n.id) || (n.tipo === "tag" && tagsConectados.has(n.id))
      );
    }

    const idsValidos = new Set(nodos.map((n) => n.id));
    const enlaces = grafoCrudo.links.filter(
      (l) => idsValidos.has(l.source) && idsValidos.has(l.target)
    );

    // Clonar para que force-graph pueda mutar (agrega x, y, vx, vy...) sin
    // tocar el JSON original, así los filtros se pueden recalcular limpios.
    return {
      nodes: nodos.map((n) => ({ ...n })),
      links: enlaces.map((l) => ({ ...l })),
    };
  }

  function terminoBusquedaActual() {
    return elBusqueda ? normalizar(elBusqueda.value.trim()) : "";
  }

  function coincideBusqueda(nodo, termino) {
    if (!termino) return true;
    return normalizar(nodo.label || "").includes(termino);
  }

  function mostrarInfo(nodo) {
    if (!nodo) {
      elInfo.textContent = "Selecciona un nodo para ver detalles.";
      return;
    }
    if (nodo.tipo === "apunte") {
      elInfo.innerHTML = `<strong>${nodo.label}</strong> &middot; ${nodo.archivo} &middot; ${nodo.curso} / ${nodo.categoria}`;
    } else if (nodo.tipo === "categoria") {
      elInfo.innerHTML = `<strong>${nodo.label}</strong> (tema) &middot; curso: ${nodo.curso}`;
    } else if (nodo.tipo === "tag") {
      elInfo.innerHTML = `<strong>${nodo.label}</strong> (etiqueta transversal)`;
    } else {
      elInfo.innerHTML = `<strong>${nodo.label}</strong> (curso)`;
    }
  }

  function inicializarGrafo() {
    if (instanciaGrafo || typeof ForceGraph === "undefined") return;

    instanciaGrafo = ForceGraph()(elLienzo)
      .backgroundColor("rgba(0,0,0,0)")
      .nodeId("id")
      .nodeVal((n) => TAMANOS[n.tipo] || 3)
      .nodeColor((n) => {
        const termino = terminoBusquedaActual();
        if (termino && !coincideBusqueda(n, termino)) return "rgba(120,124,140,0.15)";
        return COLORES[n.tipo] || "#ccc";
      })
      .nodeLabel((n) => n.label)
      .linkColor(() => "rgba(154,160,180,0.25)")
      .linkWidth(0.6)
      .onNodeClick((nodo) => {
        mostrarInfo(nodo);
        if (nodo.tipo === "apunte") {
          window.open(encodeURI(nodo.ruta), "_blank", "noopener");
        } else if (nodo.tipo === "curso" && elFiltroCurso) {
          elFiltroCurso.value = nodo.label;
          elFiltroCurso.dispatchEvent(new Event("change"));
          actualizarGrafo();
        }
      })
      .onBackgroundClick(() => mostrarInfo(null))
      .cooldownTicks(150);

    ajustarTamanoLienzo();
    window.addEventListener("resize", ajustarTamanoLienzo);
  }

  function ajustarTamanoLienzo() {
    if (!instanciaGrafo) return;
    const rect = elLienzo.getBoundingClientRect();
    instanciaGrafo.width(rect.width).height(Math.max(420, rect.width * 0.65));
  }

  function actualizarGrafo() {
    if (!instanciaGrafo) return;
    instanciaGrafo.graphData(construirSubgrafo());
  }

  function cargarDatosGrafo() {
    if (grafoCrudo || cargando) return Promise.resolve();
    cargando = true;
    return fetch("assets/graph.json")
      .then((r) => r.json())
      .then((json) => {
        grafoCrudo = json;
      })
      .catch(() => {
        elInfo.textContent = "No se pudo cargar el grafo de conexiones.";
      })
      .finally(() => {
        cargando = false;
      });
  }

  function activarGrafo() {
    grafoActivo = true;
    elToggle.textContent = "Modo lista";
    elToggle.setAttribute("aria-pressed", "true");
    elResultados.hidden = true;
    elContador.hidden = true;
    elVistaGrafo.hidden = false;

    cargarDatosGrafo().then(() => {
      inicializarGrafo();
      actualizarGrafo();
      ajustarTamanoLienzo();
    });
  }

  function desactivarGrafo() {
    grafoActivo = false;
    elToggle.textContent = "Modo grafo";
    elToggle.setAttribute("aria-pressed", "false");
    elResultados.hidden = false;
    elContador.hidden = false;
    elVistaGrafo.hidden = true;
  }

  elToggle.addEventListener("click", () => {
    if (grafoActivo) desactivarGrafo();
    else activarGrafo();
  });

  checkboxesTipo.forEach((cb) => cb.addEventListener("change", actualizarGrafo));
  if (elReset) {
    elReset.addEventListener("click", () => {
      if (elFiltroCurso) {
        elFiltroCurso.value = "";
        elFiltroCurso.dispatchEvent(new Event("change"));
      }
      actualizarGrafo();
      if (instanciaGrafo) instanciaGrafo.zoomToFit(400, 40);
    });
  }
  if (elFiltroCurso) {
    elFiltroCurso.addEventListener("change", () => {
      if (grafoActivo) actualizarGrafo();
    });
  }
  if (elBusqueda) {
    elBusqueda.addEventListener("input", () => {
      if (grafoActivo && instanciaGrafo) instanciaGrafo.nodeColor(instanciaGrafo.nodeColor());
    });
  }
})();
