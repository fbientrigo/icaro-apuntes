(function () {
  const elBusqueda = document.getElementById("busqueda");
  const elFiltroCurso = document.getElementById("filtroCurso");
  const elFiltroCategoria = document.getElementById("filtroCategoria");
  const elFiltroTipo = document.getElementById("filtroTipo");
  const elResultados = document.getElementById("resultados");
  const elContador = document.getElementById("contador");

  let datos = { cursos: [], items: [] };

  function escaparHtml(texto) {
    const div = document.createElement("div");
    div.textContent = texto;
    return div.innerHTML;
  }

  function normalizar(texto) {
    return texto
      .toLowerCase()
      .normalize("NFD")
      .replace(/[̀-ͯ]/g, "");
  }

  function poblarFiltros() {
    for (const curso of datos.cursos) {
      const opt = document.createElement("option");
      opt.value = curso;
      opt.textContent = curso;
      elFiltroCurso.appendChild(opt);
    }
    const tipos = [...new Set(datos.items.map((i) => i.tipo))].sort();
    for (const tipo of tipos) {
      const opt = document.createElement("option");
      opt.value = tipo;
      opt.textContent = tipo;
      elFiltroTipo.appendChild(opt);
    }
    poblarCategorias();
  }

  function poblarCategorias() {
    const cursoActual = elFiltroCurso.value;
    const categoriaPrevia = elFiltroCategoria.value;
    const base = cursoActual ? datos.items.filter((i) => i.curso === cursoActual) : datos.items;
    const categorias = [...new Set(base.map((i) => i.categoria))].sort();

    elFiltroCategoria.innerHTML = '<option value="">Todos los temas</option>';
    for (const categoria of categorias) {
      const opt = document.createElement("option");
      opt.value = categoria;
      opt.textContent = categoria;
      elFiltroCategoria.appendChild(opt);
    }
    if (categorias.includes(categoriaPrevia)) {
      elFiltroCategoria.value = categoriaPrevia;
    }
  }

  function filtrar() {
    const termino = normalizar(elBusqueda.value.trim());
    const curso = elFiltroCurso.value;
    const categoria = elFiltroCategoria.value;
    const tipo = elFiltroTipo.value;

    return datos.items.filter((item) => {
      if (curso && item.curso !== curso) return false;
      if (categoria && item.categoria !== categoria) return false;
      if (tipo && item.tipo !== tipo) return false;
      if (!termino) return true;
      const tags = (item.tags || []).join(" ");
      const texto = normalizar(`${item.nombre} ${item.curso} ${item.categoria} ${item.carpeta} ${item.tipo} ${tags}`);
      return termino.split(/\s+/).every((palabra) => texto.includes(palabra));
    });
  }

  function render() {
    const resultados = filtrar().slice(0, 300);
    elResultados.innerHTML = "";

    if (resultados.length === 0) {
      elContador.textContent = "Sin resultados";
      elResultados.innerHTML = '<li class="vacio">No se encontro material con esos filtros.</li>';
      return;
    }

    const total = filtrar().length;
    elContador.textContent = `${total} resultado${total === 1 ? "" : "s"}${total > resultados.length ? ` (mostrando ${resultados.length})` : ""}`;

    for (const item of resultados) {
      const li = document.createElement("li");
      const href = encodeURI(item.ruta);
      const tagsHtml = (item.tags || [])
        .map((t) => `<span class="tag tag-libre">${escaparHtml(t)}</span>`)
        .join("");
      li.innerHTML = `
        <a class="item" href="${href}" target="_blank" rel="noopener">
          <div class="nombre">${escaparHtml(item.nombre)}</div>
          <div class="meta">
            <span class="tag">${escaparHtml(item.tipo)}</span>
            <span>${escaparHtml(item.curso)}</span>
            <span class="tag tag-categoria">${escaparHtml(item.categoria)}</span>
            ${tagsHtml}
          </div>
        </a>`;
      elResultados.appendChild(li);
    }
  }

  fetch("assets/material.json")
    .then((r) => r.json())
    .then((json) => {
      datos = json;
      poblarFiltros();
      render();
    })
    .catch(() => {
      elContador.textContent = "No se pudo cargar el indice de material.";
    });

  elBusqueda.addEventListener("input", render);
  elFiltroCurso.addEventListener("change", () => {
    poblarCategorias();
    render();
  });
  elFiltroCategoria.addEventListener("change", render);
  elFiltroTipo.addEventListener("change", render);
})();
