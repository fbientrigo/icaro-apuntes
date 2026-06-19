(function () {
  const elBusqueda = document.getElementById("busqueda");
  const elFiltroCurso = document.getElementById("filtroCurso");
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
  }

  function filtrar() {
    const termino = normalizar(elBusqueda.value.trim());
    const curso = elFiltroCurso.value;
    const tipo = elFiltroTipo.value;

    return datos.items.filter((item) => {
      if (curso && item.curso !== curso) return false;
      if (tipo && item.tipo !== tipo) return false;
      if (!termino) return true;
      const texto = normalizar(`${item.nombre} ${item.curso} ${item.carpeta} ${item.tipo}`);
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
      li.innerHTML = `
        <a class="item" href="${href}" target="_blank" rel="noopener">
          <div class="nombre">${escaparHtml(item.nombre)}</div>
          <div class="meta">
            <span class="tag">${escaparHtml(item.tipo)}</span>
            <span>${escaparHtml(item.curso)}</span>
            ${item.carpeta && item.carpeta !== item.curso ? `<span>${escaparHtml(item.carpeta)}</span>` : ""}
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
  elFiltroCurso.addEventListener("change", render);
  elFiltroTipo.addEventListener("change", render);
})();
