(() => {
  const nav = document.getElementById("nav");
  const navToggle = document.getElementById("navToggle");
  const navLinks = document.getElementById("navLinks");
  const firmwareCode = document.getElementById("firmwareCode");
  const copyBtn = document.getElementById("copyCode");
  let firmwareText = "";

  const onScroll = () => {
    if (!nav) return;
    nav.classList.toggle("scrolled", window.scrollY > 24);
  };

  window.addEventListener("scroll", onScroll, { passive: true });
  onScroll();

  if (navToggle && navLinks) {
    navToggle.addEventListener("click", () => {
      const open = navLinks.classList.toggle("open");
      navToggle.setAttribute("aria-expanded", String(open));
    });

    navLinks.querySelectorAll("a").forEach((link) => {
      link.addEventListener("click", () => {
        navLinks.classList.remove("open");
        navToggle.setAttribute("aria-expanded", "false");
      });
    });
  }

  // Active section highlighting
  const sections = [...document.querySelectorAll("main section[id]")];
  const menuAnchors = [...document.querySelectorAll(".nav-links a[href^='#']")];

  const setActive = () => {
    const y = window.scrollY + 120;
    let current = sections[0]?.id;
    for (const section of sections) {
      if (section.offsetTop <= y) current = section.id;
    }
    menuAnchors.forEach((a) => {
      a.classList.toggle("active", a.getAttribute("href") === `#${current}`);
    });
  };

  window.addEventListener("scroll", setActive, { passive: true });
  setActive();

  // Reveal on scroll
  const reveals = document.querySelectorAll(".reveal");
  if ("IntersectionObserver" in window) {
    const io = new IntersectionObserver(
      (entries) => {
        entries.forEach((entry) => {
          if (entry.isIntersecting) {
            entry.target.classList.add("in");
            io.unobserve(entry.target);
          }
        });
      },
      { threshold: 0.12, rootMargin: "0px 0px -40px 0px" }
    );
    reveals.forEach((el) => io.observe(el));
  } else {
    reveals.forEach((el) => el.classList.add("in"));
  }

  // Load firmware + highlight
  const loadFirmware = async () => {
    if (!firmwareCode) return;
    try {
      const res = await fetch("assets/anti_sleep_alarm.ino", { cache: "no-cache" });
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      firmwareText = await res.text();
      firmwareCode.textContent = firmwareText;
      if (window.hljs) {
        window.hljs.highlightElement(firmwareCode);
      }
    } catch (err) {
      firmwareCode.textContent =
        "// Could not load firmware file.\n// Open firmware/anti_sleep_alarm/anti_sleep_alarm.ino in the repository.";
      console.error(err);
    }
  };

  loadFirmware();

  if (copyBtn) {
    copyBtn.addEventListener("click", async () => {
      const text = firmwareText || firmwareCode?.textContent || "";
      try {
        await navigator.clipboard.writeText(text);
        copyBtn.textContent = "Copied";
        setTimeout(() => {
          copyBtn.textContent = "Copy";
        }, 1600);
      } catch {
        copyBtn.textContent = "Copy failed";
        setTimeout(() => {
          copyBtn.textContent = "Copy";
        }, 1600);
      }
    });
  }
})();
