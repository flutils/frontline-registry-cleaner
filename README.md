<div align="center">
  <img src="https://i.imgur.com/FS1iaHV.png" alt="Frontline Registry Cleaner 2.0">
  <h1 align="center">⚡ Speed Up Windows PC's ⚡</h1>
  <p align="center">Keep Windows PC's running smoothly by cleaning out junk files and registry errors. Whilst retired, the product is still one of the most potent of its type - you're welcome to use it or edit the code.</p>
  <p align="center">
    <table>
      <tr>
        <th colspan="3" align="center">Features</th>
      </tr>
      <tr>
        <td align="center" width="33%"><img src="https://i.imgur.com/Zw9rI32.png" /><br /><strong>⌚ Scan<br />Perform a scan of your system with 18 different options.</strong></td>
        <td align="center" width="33%"><img src="https://i.imgur.com/goKnV6t.png" /><br /><strong>💾 Backup<br />Backup your registry and files to protect against problems.</strong></td>
        <td align="center" width="33%"><img src="https://i.imgur.com/PwDH5B4.png" /><br /><strong>📊 Maintain<br />Keep your system in check with regular updates.</strong></td>
      </tr>
    </table>
  </p>
  <p align="center">-</p>
</div>

<p align="center">
  <a href="https://raw.githubusercontent.com/Eugeny/terminus/master/LICENSE"><img alt="GitHub" src="https://img.shields.io/github/license/eugeny/terminus.svg?label=License&style=flat-square"></a> <a href="https://travis-ci.org/Eugeny/terminus"><img alt="Travis (.org)" src="https://img.shields.io/travis/Eugeny/terminus.svg?label=CI&logo=travis&logoColor=white&style=flat-square"></a>
  <a href="https://ci.appveyor.com/project/Eugeny/terminus"><img alt="AppVeyor" src="https://img.shields.io/appveyor/ci/eugeny/terminus.svg?label=CI&logo=appveyor&logoColor=white&style=flat-square"></a>
</p>

<p align="center">
  <a href="https://github.com/Eugeny/terminus/releases/latest"><img alt="GitHub All Releases" src="https://img.shields.io/github/downloads/eugeny/terminus/total.svg?label=DOWNLOAD&logo=github&style=for-the-badge"></a> <a href="https://ci.appveyor.com/project/Eugeny/terminus/build/artifacts"><img src="https://img.shields.io/badge/download-nightly%20build-magenta.svg?logo=appveyor&style=for-the-badge"/></a> <a href="https://gitter.im/terminus-terminal/community"><img alt="Gitter" src="https://img.shields.io/gitter/room/terminus/community.svg?color=blue&logo=gitter&style=for-the-badge"></a>
</p>

<div align="center">--</div><Br />

[**Frontline Register Cleaner 2.0**](https://www.frontlinecleaner.com) (released in 2011) was distributed by several retailers in the UK.

We bought the code from a company in Ukraine, promoted it through the Internet and rarely updated it. It was a proprietary engine based on "RegCure" (popular product at the time).

The distribution version requires a licensing server to permit users to log in, which we've now taken down. To compensate, we've removed the registration module, allowing people to use the product for free (if they wish).

If you are interested in this type of product, we have built an [RMM (Remote Monitoring & Management)](https://en.wikipedia.org/wiki/Remote_monitoring_and_management) tool ([FLCleaner 3.0](https://www.flcleaner.com)), which supersedes its predecessor by a significant margin. The new product is built around our experience with web centric applications ("cloud"), permitting extensibility.

---

#### ⚠️ Requirements

1. [VC++ Redistributable (x86)](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)
2. [.NET Framework 4.8+](https://dotnet.microsoft.com/download/dotnet-framework/net48)

When running the app, you will need to an `x86` version of VC++ redistributable.

It does not seem to matter which version you use, as long as it is `x86` compatible. We found that without it, the app will fail citing `FLCleanEngine.dll` being unavailable.

---

#### ⚙️ Code Signing

3 of the projects inside the solution have "post-build" events to sign the code.

These call the signtool commands required to process the signature. They can be removed if you don't have a certificate - we are not going to distribute our certificate publicly: -

- **1️⃣ FLCleanEngine**<br />
  Signs the DLL which is created to serve the GUI: -
  
  `"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe" sign /f [[ certificate ]] /p [[ password ]] /fd SHA256 /t http://timestamp.digicert.com /a $(BuiltOuputPath)`
  
  <img src="https://i.imgur.com/7rurg3t.jpg" width="600" title="FLCleanEngine Post Build Command" />

  **Update 14/03/2022**
  
  If you open [`FLCleanEngine.vcxproj`](https://github.com/flutils/flcleaner/blob/master/FLCleanEngine/FLCleanEngine.vcxproj) in a text editor and scroll to the bottom, you'll see the certificate `PropertyGroup`. 
  
  Add your path to SignTool.exe in the `<SignToolPath>` macro, as well as populating the `<CertificatePath>` and `<CertificatePassword>` macros, the command should fire.
  
  This is how it should look at the end of the file: -
  
  ```
  <PropertyGroup>
    <CertificatePath>" --- your certificate path --- "</CertificatePath>
    <CertificatePassword>" --- your certificate password --- "</CertificatePassword>
    <SignToolPath>"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe"</SignToolPath>
    <PostBuildEvent>if not $(SignToolPath) == "" (
       $(SignToolPath) sign /f $(CertificatePath) /p $(CertificatePassword) /fd SHA256 /t http://timestamp.digicert.com /a "$(TargetPath)"
     )
    </PostBuildEvent>
  </PropertyGroup>
  <Target Name="AfterBuild">
    <Message Text="The Project File (with Notepad) Manages The Change PostBuild Event (http://pinter.org/archives/1348)." Importance="high" />
  </Target>
  ```
  
  If you use this method, you do not need to change the `PostBuild` command. 
  
  --
   
- **2️⃣ FrontLineGUI**<br />
  Signs the `FLCleaner2.0.exe` file that's created by VS in the `obj` subdirectory of its project folder. The reason for doing this is to ensure that the EXE is signed when it's added to the setup MSI: -
  
  `"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe" sign /f [[ certificate ]] /p [[ password ]] /fd SHA256 /t http://timestamp.digicert.com /a "$(ProjectDir)obj\$(ConfigurationName)\$(TargetFileName)"`
  
  <img src="https://i.imgur.com/Tk1BfOY.jpg" width="450" title="FrontlineGUI Post Build Command" />
  
  **Update 14/03/2022**
  
  If you open [`FrontlineGUI.csproj`](https://github.com/flutils/flcleaner/blob/master/FrontLineGUI/FrontLineGUI.csproj) in a text editor and scroll to the bottom, you'll see the certificate `PropertyGroup`. 
  
  Add your path to SignTool.exe in the `<SignToolPath>` macro, as well as populating the `<CertificatePath>` and `<CertificatePassword>` macros, the command should fire.
  
  This is how it should look at the end of the file: -
  
  ```
  <PropertyGroup>
    <CertificatePath>" --- your certificate path --- "</CertificatePath>
    <CertificatePassword>" --- your certificate password --- "</CertificatePassword>
    <SignToolPath>"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe"</SignToolPath>
    <PostBuildEvent>if not $(SignToolPath) == "" (
        $(SignToolPath) sign /f $(CertificatePath) /p $(CertificatePassword) /fd SHA256 /t http://timestamp.digicert.com /a "$(ProjectDir)obj\$(ConfigurationName)\$(TargetFileName)"
      )
    </PostBuildEvent>
  </PropertyGroup>
  <Target Name="AfterBuild">
    <Message Text="The Project File (with Notepad) Manages The Change PostBuild Event (http://pinter.org/archives/1348)." Importance="high" />
  </Target>
  ```
  
    If you use this method, you do not need to change the `PostBuild` command. 

  --
  
- **3️⃣ Setup**<br />
  Signed to provide a secure version of the installation software. This is signed with the following post-build event (changeable from the "Properties" menu of the Setup project
  
  `"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe" sign /f [[ certificate ]] /p [[ password ]] /fd SHA256 /t http://timestamp.digicert.com /a $(TargetPath)`
  
  <img src="https://i.imgur.com/MsyHRVq.jpg" width="600" title="Setup Post-Build Command" />
  
  As mentioned, because this does not support custom macros, you have to apply the above PostBuild command manually.
    
  --
      
 - **4️⃣ SetupActions**<br />
  Signed to ensure the new SetupActions.dll file (used to provide extra functionality to the MSI setup project) is protected at the code level: -
  
   `"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe" sign /f [[ certificate ]] /p [[ password ]] /fd SHA256 /t http://timestamp.digicert.com /a "$(ProjectDir)obj\$(ConfigurationName)\$(TargetFileName)"`
  
   <img src="https://i.imgur.com/ot0t1MX.jpg" width="450" title="Setup Post-Build Command" />
  
   **Update 14/03/2022**
  
   If you open [SetupActions.csproj](https://github.com/flutils/flcleaner/blob/master/SetupActions/SetupActions.csproj) in a text editor and scroll to the bottom, you'll see the certificate `PropertyGroup`. 
  
   Add your path to SignTool.exe in the `<SignToolPath>` macro, as well as populating the `<CertificatePath>` and `<CertificatePassword>` macros, the command should fire.
  
   This is how it should look at the end of the file: -
  
   ```
   <PropertyGroup>
     <CertificatePath>" --- your certificate path --- "</CertificatePath>
     <CertificatePassword>" --- your certificate password --- "</CertificatePassword>
     <SignToolPath>"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe"</SignToolPath>
     <PostBuildEvent>if not $(SignToolPath) == "" (
        $(SignToolPath) sign /f $(CertificatePath) /p $(CertificatePassword) /fd SHA256 /t http://timestamp.digicert.com /a "$(ProjectDir)obj\$(ConfigurationName)\$(TargetFileName)"
      )
     </PostBuildEvent>
   </PropertyGroup>
   <Target Name="AfterBuild">
     <Message Text="The Project File (with Notepad) Manages The Change PostBuild Event (http://pinter.org/archives/1348)." Importance="high" />
   </Target>
   ```
  
   If you use this method, you do not need to change the `PostBuild` command. 

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