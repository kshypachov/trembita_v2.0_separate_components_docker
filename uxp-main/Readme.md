# uxp-main Docker Image

## Description

`uxp-main` is a base Docker image that includes the installed `uxp-securityserver` (Trembita) component.  
The image is used **exclusively as a source of filesystem structure** for initializing persistent storage in Kubernetes.

> ❗ Important: All secrets (certificates, keys, passwords, etc.) are **regenerated inside Kubernetes** using CI/CD pipelines or init containers. This makes the image safe for storage and distribution — it **contains no sensitive data**.

## Purpose

This container:
- Provides the required filesystem structure and binary dependencies.
- Simplifies the initial population of volumes (`/etc/uxp`, `/var/lib/uxp`, etc...).
- Is not intended to run as a production container (it does not launch `supervisord`, and `entrypoint.sh` is disabled).

## Usage

Typically used as an init container or as a source for an `persistentVolumeClaim` with post-copying:

```yaml
initContainers:
  - name: init-uxp-main
    image: <registry>/uxp-main:latest
    command: ["sh", "-c"]
    args:
      - cp -a /etc/uxp /mnt/etc/ && cp -a /var/lib/uxp /mnt/lib
    volumeMounts:
      - name: uxp-etc
        mountPath: /mnt/etc
      - name: uxp-lib
        mountPath: /mnt/lib
```

## Contents

The image includes:
- Installed `uxp-securityserver-trembita` package
- PostgreSQL 16 (pre-installed, but inactive)
- NGINX and basic OS utilities
- `fake-systemd` to bypass systemd dependencies
- Cleaned-up default UXP configs to reduce image size

## Security

- All default configurations containing credentials or identifiers are removed
- Users `uxp` and `uxpadmin` have restricted privileges
- No secrets are embedded in the image
- Uses an internal signed APT repository (`192.168.99.247`)

## License

Author: [Kirill Shypachov](https://github.com/kshypachov)