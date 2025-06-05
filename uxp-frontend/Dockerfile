
FROM ubuntu:22.04

RUN groupadd -g 114 nginx && \
    useradd -m -u 112 -g 114 -d /var/www -s /usr/sbin/nologin -p '' nginx


RUN apt update &&  \
    apt install -qq --no-install-recommends -y nginx authbind && \
    rm -rf /var/lib/apt/lists/* &&  \
    apt clean && rm /etc/nginx/sites-enabled/default

COPY nginx.conf /etc/nginx/nginx.conf

RUN rm -fr /var/log/nginx && \
    mkdir -p /var/log/nginx && \
    ln -sf /dev/null /var/log/nginx/error.log

COPY default-uxp.conf /etc/nginx/sites-enabled/default-uxp
COPY conf.d/* /etc/nginx/conf.d/

RUN mkdir -p /usr/share/uxp/
COPY usr/share/uxp/ /usr/share/uxp/

RUN mkdir -p /var/lib/uxp/public/
COPY public/ /var/lib/uxp/public/

RUN mkdir -p /tmp/nginx/ && \
    chown -R nginx:nginx /tmp/nginx/

RUN touch /etc/authbind/byport/80 && chmod 112 /etc/authbind/byport/80 && chown 112 /etc/authbind/byport/80

RUN rm /bin/sh && rm /bin/bash
USER nginx
ENTRYPOINT ["authbind", "nginx", "-g", "daemon off;"]