#!/usr/bin/env python3
"""
Generate a static HTML index of all PDF files in the repository.
Outputs to _site/index.html and copies PDFs to _site/ preserving paths.
Run from the repository root.
"""

import os
import shutil
from pathlib import Path
from urllib.parse import quote

REPO_ROOT = Path(__file__).resolve().parent.parent.parent
SITE_DIR = REPO_ROOT / "_site"
SKIP_DIRS = {".git", ".github", "_site", "node_modules", ".venv"}


def collect_pdfs(root: Path) -> dict:
    """
    Returns a nested dict: { top_folder: { relative_path: [pdf_paths] } }
    Top-level PDFs are grouped under a special key "."
    """
    tree: dict = {}

    for dirpath, dirnames, filenames in os.walk(root):
        # Prune ignored directories in-place
        dirnames[:] = sorted(
            d for d in dirnames if d not in SKIP_DIRS and not d.startswith(".")
        )

        pdf_files = sorted(f for f in filenames if f.lower().endswith(".pdf"))
        if not pdf_files:
            continue

        rel_dir = Path(dirpath).relative_to(root)
        parts = rel_dir.parts

        # Top-level key = first folder component, or "." for root PDFs
        top = parts[0] if parts else "."

        if top not in tree:
            tree[top] = {}

        tree[top].setdefault(str(rel_dir), []).extend(
            [rel_dir / f for f in pdf_files]
        )

    return tree


def url_path(p: Path) -> str:
    """Convert a Path to a URL-safe string with forward slashes."""
    return "/".join(quote(part) for part in p.parts)


def render_html(tree: dict) -> str:
    total = sum(len(files) for dirs in tree.values() for files in dirs.values())

    sections: list[str] = []
    for top_folder in sorted(tree.keys()):
        dirs = tree[top_folder]
        folder_pdfs = sum(len(files) for files in dirs.values())
        section_id = quote(top_folder, safe="")

        # Build file list grouped by sub-directory
        items: list[str] = []
        for rel_dir in sorted(dirs.keys()):
            pdfs = dirs[rel_dir]
            sub = Path(rel_dir)
            # Show subfolder label only if it differs from the top folder
            if str(sub) != top_folder and sub.parts:
                sub_label = " / ".join(sub.parts[1:]) if len(sub.parts) > 1 else ""
                if sub_label:
                    items.append(
                        f'<li class="subfolder-label">📁 {sub_label}</li>'
                    )
            for pdf in pdfs:
                name = pdf.name
                href = url_path(pdf)
                items.append(
                    f'<li class="pdf-item" data-name="{name.lower()}" data-folder="{top_folder.lower()}">'
                    f'<a href="{href}" target="_blank" rel="noopener">📄 {name}</a>'
                    f"</li>"
                )

        items_html = "\n          ".join(items)
        sections.append(
            f"""
    <details class="course-section" id="{section_id}">
      <summary>
        <span class="course-name">{top_folder}</span>
        <span class="pdf-count">{folder_pdfs} PDF{'s' if folder_pdfs != 1 else ''}</span>
      </summary>
      <ul class="pdf-list">
          {items_html}
      </ul>
    </details>"""
        )

    sections_html = "\n".join(sections)

    return f"""<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Ícaro — Apuntes</title>
  <style>
    *, *::before, *::after {{ box-sizing: border-box; margin: 0; padding: 0; }}

    :root {{
      --bg: #0f1117;
      --surface: #1a1d27;
      --border: #2e3148;
      --accent: #7c83f5;
      --accent-hover: #a5abff;
      --text: #e2e4f0;
      --muted: #7a7f9a;
      --pdf-link: #9ecaff;
      --subfolder: #555c7a;
    }}

    body {{
      background: var(--bg);
      color: var(--text);
      font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
      min-height: 100vh;
      padding: 2rem 1rem 4rem;
    }}

    header {{
      text-align: center;
      margin-bottom: 2.5rem;
    }}
    header h1 {{
      font-size: 2.4rem;
      font-weight: 700;
      letter-spacing: -0.5px;
      color: var(--accent-hover);
    }}
    header p {{
      color: var(--muted);
      margin-top: 0.4rem;
      font-size: 0.95rem;
    }}

    .search-wrapper {{
      max-width: 640px;
      margin: 0 auto 2rem;
    }}
    #search {{
      width: 100%;
      padding: 0.75rem 1rem;
      border-radius: 8px;
      border: 1px solid var(--border);
      background: var(--surface);
      color: var(--text);
      font-size: 1rem;
      outline: none;
      transition: border-color 0.2s;
    }}
    #search:focus {{ border-color: var(--accent); }}
    #search::placeholder {{ color: var(--muted); }}

    .stats {{
      text-align: center;
      color: var(--muted);
      font-size: 0.85rem;
      margin-bottom: 1.5rem;
    }}

    .courses {{
      max-width: 860px;
      margin: 0 auto;
      display: flex;
      flex-direction: column;
      gap: 0.6rem;
    }}

    details.course-section {{
      background: var(--surface);
      border: 1px solid var(--border);
      border-radius: 10px;
      overflow: hidden;
      transition: border-color 0.2s;
    }}
    details.course-section[open] {{
      border-color: var(--accent);
    }}

    summary {{
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 0.85rem 1.1rem;
      cursor: pointer;
      user-select: none;
      list-style: none;
      gap: 1rem;
    }}
    summary::-webkit-details-marker {{ display: none; }}
    summary::before {{
      content: '▶';
      color: var(--muted);
      font-size: 0.7rem;
      transition: transform 0.2s;
      flex-shrink: 0;
    }}
    details[open] > summary::before {{ transform: rotate(90deg); }}

    .course-name {{
      flex: 1;
      font-weight: 600;
      font-size: 0.95rem;
      color: var(--text);
    }}
    .pdf-count {{
      font-size: 0.8rem;
      color: var(--muted);
      background: var(--bg);
      padding: 0.2rem 0.55rem;
      border-radius: 20px;
      white-space: nowrap;
    }}

    .pdf-list {{
      list-style: none;
      padding: 0.3rem 0 0.8rem 2.4rem;
      border-top: 1px solid var(--border);
    }}

    .pdf-list li {{
      padding: 0.18rem 0;
    }}
    .pdf-list li.subfolder-label {{
      font-size: 0.78rem;
      color: var(--subfolder);
      margin-top: 0.6rem;
      padding-left: 0;
    }}
    .pdf-list li.pdf-item a {{
      color: var(--pdf-link);
      text-decoration: none;
      font-size: 0.88rem;
      word-break: break-word;
      transition: color 0.15s;
    }}
    .pdf-list li.pdf-item a:hover {{ color: var(--accent-hover); text-decoration: underline; }}

    .hidden {{ display: none !important; }}

    #no-results {{
      text-align: center;
      color: var(--muted);
      margin-top: 2rem;
      display: none;
    }}

    footer {{
      text-align: center;
      color: var(--muted);
      font-size: 0.8rem;
      margin-top: 3rem;
    }}
  </style>
</head>
<body>
  <header>
    <h1>📚 Ícaro — Apuntes</h1>
    <p>Índice de apuntes y materiales de estudio</p>
  </header>

  <div class="search-wrapper">
    <input id="search" type="search" placeholder="🔍 Buscar apunte o ramo…" autocomplete="off" />
  </div>

  <p class="stats">{total} archivos PDF en {len(tree)} ramos</p>

  <div class="courses" id="courses-container">
{sections_html}
  </div>
  <p id="no-results">No se encontraron resultados.</p>

  <footer>
    Generado automáticamente · <a href="https://github.com/fbientrigo/icaro-apuntes" style="color:var(--accent)">fbientrigo/icaro-apuntes</a>
  </footer>

  <script>
    const searchInput = document.getElementById('search');
    const noResults   = document.getElementById('no-results');

    searchInput.addEventListener('input', () => {{
      const q = searchInput.value.trim().toLowerCase();
      let visible = 0;

      document.querySelectorAll('details.course-section').forEach(section => {{
        const folderName = section.querySelector('.course-name').textContent.toLowerCase();
        const items = section.querySelectorAll('li.pdf-item');
        let sectionVisible = false;

        if (!q) {{
          items.forEach(li => li.classList.remove('hidden'));
          section.classList.remove('hidden');
          section.removeAttribute('open');
          return;
        }}

        items.forEach(li => {{
          const name   = li.dataset.name   || '';
          const folder = li.dataset.folder || '';
          const match  = name.includes(q) || folder.includes(q);
          li.classList.toggle('hidden', !match);
          if (match) sectionVisible = true;
        }});

        // Also match on folder name itself
        if (folderName.includes(q)) {{
          items.forEach(li => li.classList.remove('hidden'));
          sectionVisible = true;
        }}

        section.classList.toggle('hidden', !sectionVisible);
        if (sectionVisible) {{ section.setAttribute('open', ''); visible++; }}
      }});

      noResults.style.display = (q && visible === 0) ? 'block' : 'none';
    }});
  </script>
</body>
</html>
"""


def copy_pdfs(tree: dict, root: Path, site_dir: Path) -> None:
    """Copy all PDF files to site_dir preserving relative paths."""
    for dirs in tree.values():
        for rel_dir, pdfs in dirs.items():
            dest_dir = site_dir / rel_dir
            dest_dir.mkdir(parents=True, exist_ok=True)
            for pdf_rel in pdfs:
                src = root / pdf_rel
                dst = site_dir / pdf_rel
                if src.exists():
                    shutil.copy2(src, dst)


def main() -> None:
    print(f"Repository root: {REPO_ROOT}")
    print(f"Output site dir: {SITE_DIR}")

    SITE_DIR.mkdir(parents=True, exist_ok=True)

    print("Collecting PDF files…")
    tree = collect_pdfs(REPO_ROOT)
    total = sum(len(files) for dirs in tree.values() for files in dirs.values())
    print(f"Found {total} PDFs across {len(tree)} top-level folders.")

    print("Generating index.html…")
    html = render_html(tree)
    (SITE_DIR / "index.html").write_text(html, encoding="utf-8")

    print("Copying PDFs to _site/…")
    copy_pdfs(tree, REPO_ROOT, SITE_DIR)

    print("Done ✓")


if __name__ == "__main__":
    main()
