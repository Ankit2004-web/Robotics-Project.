# Website

Production-style static portfolio site for the Anti Sleep Alarm project.

```text
website/
├── index.html          ← open this page
├── css/styles.css      ← logo-themed styles
├── js/app.js           ← nav, animations, firmware loader
├── logo.png
├── assets/             ← self-contained images + firmware copy
└── README.md
```

## Local preview

From the repository root:

```bash
python -m http.server 8080
```

Open: [http://localhost:8080/website/](http://localhost:8080/website/)

## GitHub Pages

1. Settings → Pages  
2. Branch: `main`  
3. Folder: `/website`  

## Push target

Only: `https://github.com/Ankit2004-web/anti-sleep-alarm`
