FROM ubuntu:24.04

LABEL authors="Kirill Shypachov (@kshypachov), on behalf of eGA Kyiv"
LABEL org.opencontainers.image.authors="Kirill Shypachov <kiril.shypachov@ega.ee> (eGA Kyiv)"
LABEL org.opencontainers.image.vendor="eGA Kyiv"
LABEL org.opencontainers.image.title="trembita monitor agent"
LABEL org.opencontainers.image.url="URI documentation"
LABEL org.opencontainers.image.source=""
LABEL org.opencontainers.image.version=""

RUN groupadd -g 114 nginx && \
    useradd -m -u 112 -g 114 -d /var/www -s /usr/sbin/nologin -p '' nginx


RUN apt update &&  \
    apt install -qq --no-install-recommends -y nginx && \
    rm -rf /var/lib/apt/lists/* &&  \
    apt clean && rm /etc/nginx/sites-enabled/default

COPY nginx.conf         /etc/nginx/nginx.conf
COPY stream-proxy.conf  /etc/nginx/conf.d/

#USER nginx
ENTRYPOINT [ "nginx", "-g", "daemon off;"]
