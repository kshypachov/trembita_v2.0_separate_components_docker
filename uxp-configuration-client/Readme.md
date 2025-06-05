# uxp-configuration-client

`uxp-configuration-client` is a key component of the Trembita 2.0 system responsible for downloading, verifying, and storing **global configuration** from external sources (spesified in anchor). Other components such as `uxp-proxy` rely on this configuration to operate securely and correctly.

---

## 📡 Behavior in Monolithic UXP Deployment

- **Listens on port `5666`** for management requests (e.g. triggered via web interface).
- Example request used by the frontend to trigger re-download of configuration:

```http
GET /execute HTTP/1.1
Host: localhost:5666
User-Agent: Apache-HttpClient/5.3.1 (Java/17.0.15)
Accept: text/plain, application/json
```

---

## 📁 Key Files and Directories

| Path                                             | Purpose                                              |
|--------------------------------------------------|------------------------------------------------------|
| `/etc/uxp/services/confclient.conf`             | Main startup config for the client                  |
| `/etc/uxp/services/global.conf`                 | Referenced by `confclient.conf`                     |
| `/etc/uxp/services/local.conf`                  | Referenced by `confclient.conf`                     |
| `/etc/uxp/conf.d/confclient-logback.xml`        | Logback configuration for the Java process          |
| `/usr/share/uxp/jlib/addon/confclient/*.conf`   | Additional addon configurations                     |
| `/usr/share/uxp/bin/confclient.sh`              | Legacy startup script                               |

---
<details>
<summary><strong>Click to expand full startup log trace</strong></summary>

```bash
+ . /etc/uxp/services/confclient.conf
++ . /etc/uxp/services/global.conf
++ JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
++ '[' -z /usr/share/uxp/jlib/addon ']'
++ for addon in ${ADDON_PATH}/confclient/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/confclient/trembita-crypto.conf ']'
++ . /usr/share/uxp/jlib/addon/confclient/trembita-crypto.conf
+++ ADDON_CP=':/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export 'LD_PRELOAD= /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ LD_PRELOAD=' /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/configuration-client.jar
++ CONFCLIENT_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/confclient-logback.xml'
++ CONFCLIENT_JVM_OPTS=' -Xmx50m -XX:MaxMetaspaceSize=70m '
++ . /etc/uxp/services/local.conf
++ CONFCLIENT_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/confclient-logback.xml'
+ date -R
Tue, 27 May 2025 19:46:29 +0000

# Final execution command:
exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java \
  -Xmx50m \
  -XX:MaxMetaspaceSize=70m \
  -Dlogback.configurationFile=/etc/uxp/conf.d/confclient-logback.xml \
  -XX:+UseG1GC \
  -Xshare:on \
  -Dfile.encoding=UTF-8 \
  -Djava.library.path=/usr/share/uxp/lib/ \
  -cp '/usr/share/uxp/jlib/configuration-client.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' \
  -Dorg.bytedeco.javacpp.noPointerGC=true \
  -Dorg.bytedeco.javacpp.maxBytes=0 \
  -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 \
  ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain
```
</details>

---

## 📦 Mounted Paths (Required for Running)

| Path               | Purpose                                                                      |
|--------------------|-------------------------------------------------------------------------------|
| `/etc/uxp/`        | Used to store downloaded global configuration and load anchor/config files   |
| `/usr/share/uxp/`  | Required for loading additional configuration and JARs                       |
| `/var/tmp/uxp/`    | Possibly used as a temporary location during uninitialized state             |

> ✅ All runtime state is isolated to mounted paths.  
> ❌ The container **has no shell** and **runs with a read-only filesystem**.

---

## 🚀 Application Startup (Extracted Flow)

When launched (e.g. via `confclient.sh`), the process:

1. Loads `/etc/uxp/services/confclient.conf`
2. Sources additional configs: `global.conf`, `local.conf`, addons
3. Sets environment variables like `JAVA_HOME`, `LD_LIBRARY_PATH`
4. Constructs a full Java command with all required `-D` options and classpath
5. Executes the Java entrypoint:

```bash
ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain
```

---

## 🐳 Docker Runtime (no ENTRYPOINT)

Containers are built **without ENTRYPOINT**. If running manually, add one:

```dockerfile
ENTRYPOINT ["java", \
  "-Xmx50m", \
  "-XX:MaxMetaspaceSize=70m", \
  "-XX:+UseG1GC", \
  "-Xshare:on", \
  "-Dfile.encoding=UTF-8", \
  "-Djava.library.path=/app/lib", \
  "-Dlogback.configurationFile=/app/confclient-logback.xml", \
  "-Dorg.bytedeco.javacpp.noPointerGC=true", \
  "-Dorg.bytedeco.javacpp.maxBytes=0", \
  "-Duxp.configuration-client.port=5665", \
  "-Duxp.common.temp-files-path=/etc/uxp/tmp/", \
  "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
  "-cp", "/app/configuration-client.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*", \
  "ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain"]
```

---

## ☸️ Running in Kubernetes

Use the following `command` and `args` in your Pod or container spec:

```yaml
command: ["/opt/java/openjdk/bin/java"]
args:
  - "-Xmx50m"
  - "-XX:MaxMetaspaceSize=70m"
  - "-XX:+UseG1GC"
  - "-Xshare:on"
  - "-Dorg.bytedeco.javacpp.cachedir=/tmp/java"
  - "-Dfile.encoding=UTF-8"
  - "-Djava.library.path=/app/lib"
  - "-Dlogback.configurationFile=/app/confclient-logback.xml"
  - "-Dorg.bytedeco.javacpp.noPointerGC=true"
  - "-Dorg.bytedeco.javacpp.maxBytes=0"
  - "-Duxp.configuration-client.port=5665"
  - "-Duxp.common.temp-files-path=/etc/uxp/tmp/"
  - "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0"
  - "-cp"
  - "/app/configuration-client.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*"
  - "ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain"
```
### 📂 Writable Paths in Read-Only Filesystem

The container is designed to run on a **read-only root filesystem**.  
Only the following mounted paths are writable and required for proper operation:

| Mount Path       | Purpose                                                 |
|------------------|----------------------------------------------------------|
| `/etc/uxp/`      | Stores downloaded global configuration (`globalconf`)    |
| `/tmp/java/`     | Temporary directory used by the JVM and `javacpp`        |
| `/var/tmp/uxp/`  | Possibly used during initial bootstrap (exact purpose unknown) |

> ⚠️ All other filesystem locations are **read-only** and **must not** be used for writing.

> 🧊 The container operates in a hardened mode: minimal permissions, non-root user, and a read-only root filesystem.

---

Let me know if you’d like this rendered as a standalone `README.md` file or integrated into the main repo docs!