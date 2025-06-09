# 🌐 UXP-Frontend

**UXP-frontend** is a service responsible for connecting the WebUI to the security server backend services via HTTP reverse proxy. It routes all WebUI and API calls to the following components:

- `uxp-verifier`
- `uxp-identity-provider`
- `uxp-seg-rest-api`

The service is implemented using **nginx** and acts as the main **entry point** for all HTTP-based interactions with the security server.

## 📁 Structure and Key Configuration Changes

### 📌 Nginx Configuration

| File | Description |
|------|-------------|
| `/etc/nginx/nginx.conf` | Modified to send logs to `stdout/stderr` instead of log files (suitable for containers and Kubernetes) |
| `/etc/nginx/sites-enabled/default-uxp` | Port changed from `4000` to `80`. HTTPS termination is disabled — it is now handled by Ingress |
| `/etc/nginx/conf.d/uxp-versions.include` | New config file that serves the list of installed versions. Required for the WebUI to work properly after Kubernetes migration |
| `/etc/nginx/conf.d/` | Other files modified to disable HTTP-to-HTTPS redirects. Redirect logic is now delegated to Ingress. All disabled options are commented out, not removed |

### 📦 UI and Documentation Files

- `/usr/share/uxp/` — static WebUI files
- `/var/lib/uxp/public/` — help and documentation content

## 🔐 Security and Execution Notes

- `nginx` runs as a **non-privileged user**
- The container is **shell-less** (no `/bin/sh`)
- Uses `authbind` to bind port `80` without requiring root privileges

## 🚀 Usage in Kubernetes

- HTTPS termination is performed by the **Ingress controller**, the container handles only HTTP
- The service logs to `stdout` and does not require shell access — fully container/Kubernetes ready
- The `uxp-versions.include` file is a **temporary workaround** to ensure WebUI version checks continue to function after containerization

## 🧪 Verification Checklist

After the container starts:

- Ensure port `80` is open and being listened to
- `GET /versions` endpoint should return correct component version info
- WebUI should be accessible via the Ingress endpoint

## 📄 License

Distributed as part of the UXP platform. Usage is subject to the licensing terms of the Trembita.