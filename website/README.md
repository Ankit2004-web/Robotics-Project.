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

## Deploy on Vercel (recommended)

No CLI install needed — use the Vercel website:

1. Go to [https://vercel.com/new](https://vercel.com/new)
2. Sign in with GitHub (`Ankit2004-web`)
3. Import repository: **anti-sleep-alarm**
4. Configure:
   - **Framework Preset:** Other
   - **Root Directory:** `website`
   - **Build Command:** leave empty
   - **Output Directory:** leave empty / `.`
5. Click **Deploy**

Repo root also includes `vercel.json` with `outputDirectory: "website"` as a fallback.

After deploy, Vercel gives a URL like:
`https://anti-sleep-alarm.vercel.app`

## GitHub Pages (optional)

1. Settings → Pages  
2. Branch: `main`  
3. Folder: `/website`  

## Push target

Only: `https://github.com/Ankit2004-web/anti-sleep-alarm`
