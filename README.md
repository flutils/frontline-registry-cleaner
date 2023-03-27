<img src="Readme/screenshot_0.png" alt="Frontline Registry Cleaner 2.1">

<p align="center">
  <a href="https://raw.githubusercontent.com/Eugeny/terminus/master/LICENSE"><img alt="GitHub" src="https://img.shields.io/github/license/eugeny/terminus.svg?label=License&style=flat-square"></a> <a href="https://travis-ci.org/Eugeny/terminus"><img alt="Travis (.org)" src="https://img.shields.io/travis/Eugeny/terminus.svg?label=CI&logo=travis&logoColor=white&style=flat-square"></a>
  <a href="https://ci.appveyor.com/project/Eugeny/terminus"><img alt="AppVeyor" src="https://img.shields.io/appveyor/ci/eugeny/terminus.svg?label=CI&logo=appveyor&logoColor=white&style=flat-square"></a>
</p>

<p align="center">
  <a href="https://github.com/Eugeny/terminus/releases/latest"><img alt="GitHub All Releases" src="https://img.shields.io/github/downloads/eugeny/terminus/total.svg?label=DOWNLOAD&logo=github&style=for-the-badge"></a> <a href="https://ci.appveyor.com/project/Eugeny/terminus/build/artifacts"><img src="https://img.shields.io/badge/download-nightly%20build-magenta.svg?logo=appveyor&style=for-the-badge"/></a> <a href="https://gitter.im/terminus-terminal/community"><img alt="Gitter" src="https://img.shields.io/gitter/room/terminus/community.svg?color=blue&logo=gitter&style=for-the-badge"></a>
</p>

<div align="center">--</div><Br />

[Frontline Registry Cleaner 2.0](https://www.frontlinecleaner.com) was released in 2011 and distributed by several retailers in the UK.

It enables Windows PC users to clean out junk files and corrupted registry settings with a simple scanning system.

In 2022, we revamped the GUI and some of the backend scanning functionality to bring it up to modern standards. We intend to continue to add to this functionality to embrace cloud endpoints and various other additions.

---

#### ℹ️ How It Works

The redesigned version permits "items" to be created.

These are what the tool scans and come in three types - `settings`, `files` and `security`. 

Users are able to add and manage the "items" they wish to scan, allowing for customizability.

In the future, we want to add the means to include "plugins" that permit pre-packaged items to be added. 

---

#### ⚠️ Requirements

1. [VC++ Redistributable (x86)](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)
2. [.NET Core 6.0+](https://dotnet.microsoft.com/en-us/download/dotnet/6.0) (x86 or x64 Windows SDK)

When running the app, you will need to an `x86` version of VC++ redistributable.

It does not seem to matter which version you use, as long as it is `x86` compatible. We found that without it, the app will fail citing `FLCleanEngine.dll` being unavailable.

---

#### 🚦 Github Actions

We have now moved the build pipeline to Github Actions.

The build script is triggered when a `tag` is pushed to the repository: -

```
git tag v2.0.0.3-alpha1
git push origin v2.0.0.3-alpha1
```

If you wish to remove the tag, you need to use the following: -

```
git tag -d v2.0.0.3-alpha1
git push -d origin v2.0.0.3-alpha1
```

The build creates a [release](https://github.com/flutils/flcleaner/releases), which contains MSI setup applications for x64 and x86 binaries, as well as zip files with the raw executable & DLL files inside.

The code only runs in x86, so you can ignore x64 entirely.

We're working on integrating signing the various components that require it: -

 - Setup
 - SetupActions (creates a DLL for Setup)
 - FrontlineGUI (main app)
 - FLCleanEngine (cleaner engine DLL)

---

#### 🗓️ Changelog

- [x] (25/01/2022) Upgraded UI to HD
- [x] (24/01/2022) Added Setup project to deploy Win32 binaries, added MSIX for Microsoft Store submission
- [x] (27/09/2020) Changed .NET target to 4.7, VS Studio 2019, Windows SDK
- [x] (01/01/2019) Removed registration engine
- [x] (17/02/2011) Major release (production)

---

#### ✔️ Contributions

To contribute, please follow the steps below:

1. Fork the repo and clone it locally
2. Make the changes you want/need
3. Commit your local repo and push to your remote repository
4. Create a PR to bring the new changes into our master branch

Anyone is welcome to contribute whatever they wish.

As the product has been discontinued for some time, we are no longer actively developing it.

---

#### ⚖️ Copyrights, License etc

We own all copyrights to the user interface and product design.

By publishing the repository, and adopting the [MIT license](LICENSE), we have extended these rights to the community.

Anyone who wishes to use, adapt, change, parody or mimic the design may do so (within the scope of fair use). Any malicious interpretations of our code, engine and interface will not be tolerated and will be met with appropriate recourse. Our code is provided in good faith, as a gesture of good will to the community.

If you notice any of our products used in an illegal way, you should send an alert, either as an [issue](issues), or via our abuse email [abuse@frontlineutilities.co.uk](mailto:abuse@frontlineutilities.co.uk).

---

:copyright: <a href="http://www.frontlineutilities.co.uk" align="absmiddle"><img src="https://i.imgur.com/xwejn02.jpg" height="22" align="absmiddle" title="Frontline Utilities LTD"  /></a> <a href="http://github.com/richpeck" align="absmiddle" ><img src="https://avatars0.githubusercontent.com/u/1104431" height="22" align="absmiddle" title="Contributors - R Peck" /></a>



<!-- ################################### -->
<!-- ################################### -->

<!-- Images -->
[fl]:   https://raw.githubusercontent.com/flutils/flcleaner/master/3.0/Private/Readme/fl.jpg
[main]: Readme/main.jpeg

<!-- Links -->
[flutils]:              http://www.frontlineutilities.co.uk
[flcleaner.com]:        https://www.flcleaner.com
[frontlinecleaner.com]: https://www.flcleaner.com/2.0

<!-- ################################### -->
<!-- ################################### -->