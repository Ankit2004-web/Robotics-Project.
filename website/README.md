# Documentation Website

Production-style static portfolio site for the Anti Sleep Alarm project.

## Contents

- Hero with project branding (logo color theme)
- Overview + prototype photo
- System architecture diagram
- Hardware / pin tables
- Detection logic states
- Circuit + flowchart diagrams
- Full Arduino firmware viewer (syntax highlighted, copy button)
- Demo video links
- Run steps, safety notes, contribution

## Local preview

From the repository root (recommended so shared assets resolve if needed):

```bash
python -m http.server 8080
```

Open: [http://localhost:8080/website/](http://localhost:8080/website/)

Or open `website/index.html` via any static server. Firmware loads from `website/assets/anti_sleep_alarm.ino`.

## GitHub Pages

1. Repository Settings → Pages  
2. Source: Deploy from branch `main`  
3. Folder: `/website`  

Because diagrams/code are copied under `website/assets/`, Pages can serve `/website` cleanly.

## Push target

Only: `https://github.com/Ankit2004-web/anti-sleep-alarm`
