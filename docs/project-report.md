# Project Report Summary

Professional rewrite of the academic documentation for:

**Development of Autonomous Wheel Robot – Anti Sleep Alarm**

## Participants (from original documentation)

| Name | ID |
|------|----|
| Sudeep Dutta | 22052204 |
| Dipanwita Sen | 22052204 |
| Ankit Biswas | 22052533 |
| Pratik Maity | 22052133 |

> Note: The source PDF lists the same ID for two participants (Sudeep Dutta and Dipanwita Sen). This repository preserves that text as written and does not invent corrected IDs.

## Aim

Build an anti-sleep alarm demonstration where:

- a blink sensor monitors eye state
- a piezo buzzer warns after prolonged eye closure
- a hobby motor is stopped through a relay when the unsafe condition persists

## Problem Context

Driver drowsiness is a major contributor to road accidents during long drives. This academic prototype explores a low-cost embedded approach that:

1. Detects prolonged eye closure
2. Warns the operator audibly
3. Demonstrates a safety stop on a wheel robot / hobby motor

## Solution Summary

| Layer | Implementation |
|-------|----------------|
| Sensing | IR blink sensor on spectacles |
| Control | Arduino UNO firmware |
| Warning | Piezo buzzer on D12 |
| Actuation | Relay on D13 switching a 9V hobby motor |
| Demo platform | Autonomous wheel robot prototype |

## Documented Timing (Source Inconsistencies)

| Source in PDF | Warning | Motor stop |
|---------------|---------|------------|
| Theory section | ~3 seconds | ~5 seconds |
| AIM / original code | implied by code | ~4 seconds |
| Final repository firmware | **3 seconds** | **4 seconds** |

The firmware follows the **original Arduino implementation and AIM**, while clearly documenting the theory mismatch.

## Relay Logic Conclusion

From the written procedure (motor on relay **NC**) and original output pattern (`LOW` during normal run, `HIGH` during stop):

- Sketch-level assumption: **relay control behaves as active HIGH for stop**
- Implemented as `RELAY_ACTIVE_LOW = false` by default
- Made configurable because commercial modules vary

## Individual Contribution — Ankit Biswas (22052533, CSE-3)

Primary contribution documented in the source PDF: **documentation**.

Tasks described:

- Research on anti-sleep alarm concepts and applications
- Technical writing for objectives, methodology, architecture, components, and operating instructions
- Editing and formatting for clarity and organization
- Incorporating visuals/diagrams
- Review and revision with teammates

This GitHub portfolio repository extends that documentation work into a reproducible engineering package (structured docs, cleaned firmware, diagrams, and testing guidance). It does **not** claim sole hardware design/build ownership beyond what the source document states.

## Demonstration Videos

Links from the original documentation:

- Working video: https://drive.google.com/file/d/1kCcJM2rvE6KjCAY2x27UCMJ8hakX_5YS/view?usp=drive_link
- Explanation video: https://drive.google.com/file/d/19Enz9IXtXWdgx-ZpXkAhTIcgSXLgRXcD/view?usp=drive_link

Access note: both links resolve to Google Drive file pages titled `video1.mp4` and `video2.mp4`, but viewing may require the owner to grant public/link access. Treat availability as **owner-permission dependent**.

## Conclusion (from original report, restated)

The team reported that connections were completed and the anti-sleep alarm demonstration worked on the prototype hardware. This repository packages that project for clear reproduction, review, and portfolio presentation.

## Safety Classification

| Claim | Status |
|-------|--------|
| Academic / portfolio prototype | Yes |
| Low-voltage hobby demonstration | Yes |
| Certified automotive safety system | No |
| Suitable for real-vehicle brake intervention | No |
