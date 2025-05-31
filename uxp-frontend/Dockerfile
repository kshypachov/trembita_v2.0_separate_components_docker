FROM ubuntu:22.04
RUN apt update &&  \
    apt install -qq --no-install-recommends -y nginx && \
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

RUN rm /bin/sh && rm /bin/bash
#ENTRYPOINT ["top", "-b"]
ENTRYPOINT ["nginx", "-g", "daemon off;"]