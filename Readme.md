# Trembita 2.0 — Dockerized Components

This repository contains Dockerfiles and configuration files for building **separate container images** of key components in the **Trembita 2.0** system (UXP platform).  
The goal is to provide clean, reproducible, and Kubernetes-friendly images for each subsystem.

---

## 📦 Components

### 1. `uxp-proxy`
The **core component** responsible for:

- Secure message routing between Trembita 2.0 members
- All cryptographic operations:
  - Digital signature
  - Signature validation
  - Encryption
  - Decryption

This is the "heart" of the secure data exchange.

---

### 2. `uxp-configuration-client`
Responsible for:

- Downloading **global configuration** from sources defined in the anchor file
- Validating digital signatures of configuration files
- Saving the config files to directories used by other Trembita 2.0 components

---

### 3. `uxp-frontend`
Web frontend for:

- User interface (UI)
- Management API entry point

This container exposes the administrative UI of the system.

---

### 4. `uxp-identity-provider`
REST API service for **OAuth 2.0 authentication**.  
Used by:

- Web interface users
- Other management services

It provides identity data and access token issuance.

---

### 5. `uxp-main`
Base **filesystem image** of the Secure Exchange Gateway.  
Used to initialize storage volumes for other components running inside Kubernetes.

---

### 6. `uxp-message-log-archiver`
Handles **archiving of transaction logs** produced by `uxp-proxy`.  

Supports:
- Archiving to local file system
- Archiving to external S3 storage

---

### 7. `uxp-monitor`
Component for:

- Receiving **transaction logs** from `uxp-proxy`
- Forwarding logs to an **ELK stack** (e.g. Elasticsearch + Logstash)
- Performing **health monitoring** of the Secure Exchange Gateway

> ⚠️ In Kubernetes deployments, **only the health monitoring feature is not supported**.  
> Log collection and forwarding **continues to work normally**.

---

### 8. `uxp-ocsp-cache`
Caches **OCSP certificate status** for:

- `uxp-proxy`
- `uxp-seg-rest-api`

By default, OCSP checks are performed every 6 hours.

---

### 9. `uxp-seg-rest-api`
Provides **management REST API** for:

- Full control of Trembita 2.0 Secure Exchange Gateway services
- Integration with external orchestration or UI tools

---

### 10. `uxp-verifier`
REST service for:

- Querying details about past transactions processed by `uxp-proxy`
- Accessing **archived** logs stored in S3 or on the file system

---

## 📁 Structure

Each subdirectory contains:

- `Dockerfile` for the component
- Optional configuration templates
- Notes or scripts specific to that service

---

## 🧪 Usage

To build a component manually:

```bash
cd uxp-proxy
docker build -t uxp-proxy:latest .
```

> For Kubernetes use cases, images can be deployed as part of a Helm chart or static manifests.

---


## 🧩 Common Features

All components in this repository share the following characteristics:

- ✅ **Logs to stdout/stderr**  
  All logs are sent to standard output — which is the recommended best practice for containers running in **Kubernetes**. This allows seamless integration with log collectors such as Fluentd, Vector, or Loki.

- ✅ **Runs as non-root user**  
  Every container runs under the dedicated user:
  ```
  user: uxp
  ```

- 🔒 **Restricted shell access**  
  Some containers are intentionally built **without a working shell (e.g., `/bin/sh`)** to improve security posture.  
  This means you **may not be able to open an interactive shell** (`kubectl exec -it ... sh`) in every container — and that's by design.

- 📦 **Minimal external dependencies**  
  All containers are designed to be self-contained and reproducible.  
  - No external files are downloaded at runtime.  
  - No packages are installed during container startup.  
  - The only external dependency is the **base image**, which is typically one of:
    - `ubuntu:24.04`
    - `eclipse-temurin:17-jdk-jammy`

- 🏗 **Multi-stage builds**  
  All Dockerfiles use **multi-stage builds** for clean and secure final images.  
  - Build stage typically uses: `ubuntu:24.04`  
  - Final runtime stage uses: `eclipse-temurin:17-jdk-jammy` (for Java-based components)
---
> These practices ensure the containers are **secure**, **portable**, and aligned with best practices for containerized deployments in production environments.
---

## 📄 License

This repository is provided for demonstration and development purposes.  
Refer to licensing terms of the original Trembita / UXP platform components for production use.