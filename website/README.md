# Documentation Website

Lightweight static site for portfolio presentation.

## Local preview

Open `index.html` in a browser, or serve the repository root / `website` folder with any static file server so relative links to `../media` and `../diagrams` resolve.

Example:

```bash
# from repository root
python -m http.server 8080
```

Then open `http://localhost:8080/website/`.

## GitHub Pages

If this repository is on GitHub:

1. Settings → Pages
2. Source: Deploy from branch
3. Branch: `main`
4. Folder: `/website` (or root if you prefer)

Relative asset paths (`../media`, `../diagrams`) work best when Pages serves the repository root. If you set Pages to `/website` only, copy or adjust asset paths accordingly.

## Deployment status

This packaging step prepares the static site. Actual GitHub Pages enablement depends on repository settings and authentication on the publishing machine.
