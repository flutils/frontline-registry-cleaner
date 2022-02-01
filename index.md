---
layout: default
---

<div align="center">
  <a href="{{ site.github.repository_url }}/releases/latest/download/Setup.msi"><img src="{{ '/assets/images/ui.png' | relative_url }}" class="main_image" /></a>
  <p>
    <a href="{{ site.github.repository_url }}/releases/latest/download/Setup.msi" class="action_button download" {% if site.github.releases.size > 0 %}title="Version {{ site.github.latest_release.tag_name | strip | remove: 'v' }}"{% endif %}>💾 Download Installer</a>
    <a href="{{ site.github.repository_url }}" class="action_button code">⚙️ View Code</a>
  </p>

  {% if site.github.releases.size > 0 %}
    <p class="latest">
      <a href="{{ site.github.repository_url }}/releases/latest" target="_blank">{{ site.github.latest_release.tag_name | strip | remove: 'v' }}</a>
    </p>
    <br />
  {% endif %}
  --
  
  <p align="center">
  <a href="https://raw.githubusercontent.com/Eugeny/terminus/master/LICENSE"><img alt="GitHub" src="https://img.shields.io/github/license/eugeny/terminus.svg?label=License&style=flat-square"></a> <a href="https://travis-ci.org/Eugeny/terminus"><img alt="Travis (.org)" src="https://img.shields.io/travis/Eugeny/terminus.svg?label=CI&logo=travis&logoColor=white&style=flat-square"></a>
  <a href="https://ci.appveyor.com/project/Eugeny/terminus"><img alt="AppVeyor" src="https://img.shields.io/appveyor/ci/eugeny/terminus.svg?label=CI&logo=appveyor&logoColor=white&style=flat-square"></a>
</p>

<p align="center">
  <a href="https://github.com/Eugeny/terminus/releases/latest"><img alt="GitHub All Releases" src="https://img.shields.io/github/downloads/eugeny/terminus/total.svg?label=DOWNLOAD&logo=github&style=for-the-badge"></a> <a href="https://ci.appveyor.com/project/Eugeny/terminus/build/artifacts"><img src="https://img.shields.io/badge/download-nightly%20build-magenta.svg?logo=appveyor&style=for-the-badge"/></a> <a href="https://gitter.im/terminus-terminal/community"><img alt="Gitter" src="https://img.shields.io/gitter/room/terminus/community.svg?color=blue&logo=gitter&style=for-the-badge"></a>
</p>
  
</div>

---

<div align="center">
  ©️ 2009-2022 <a href="https://www.frontlineutilities.co.uk" target="_blank">Frontline Utilities LTD</a>. All rights reserved.
  <p class="sub">You'e welcome <a href="https://www.github.com/flutils/flcleaner#%EF%B8%8F-contributions">contribute to the code</a> as you want.</p>
  <a href="https://www.frontlineutilities.co.uk" target="_blank" title="Copyright Frontline Utilities LTD" class="footer-logo"><img src="https://camo.githubusercontent.com/91c37b08fa6baa4477c442d07577a58b631555371ca012f813cdb977c7a20027/68747470733a2f2f692e696d6775722e636f6d2f7877656a6e30322e6a7067" /></a>
</div>
