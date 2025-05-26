FROM ubuntu:22.04 AS bilder
ENV DEBIAN_FRONTEND=noninteractive

LABEL authors="Kirill Shypachov @kshypachov"
COPY uxp-securityserver-ui_1.4.6_all.deb /uxp-securityserver-ui_1.4.6_all.deb
RUN mkdir /tmp/ui
RUN dpkg-deb -X /uxp-securityserver-ui_1.4.6_all.deb /tmp/ui
RUN chmod 755 -R /tmp/ui

COPY uxp-identity-provider-ui_1.4.6_all.deb /uxp-identity-provider-ui_1.4.6_all.deb
RUN mkdir /tmp/identity-ui
RUN dpkg-deb -X /uxp-identity-provider-ui_1.4.6_all.deb /tmp/identity-ui
RUN chmod 755 -R /tmp/identity-ui


#ENTRYPOINT ["top", "-b"]

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

RUN mkdir  /usr/share/uxp/
RUN mkdir  /usr/share/uxp/identity-provider-ui
RUN mkdir  /usr/share/uxp/securityserver-ui
COPY --from=bilder /tmp/ui/usr/share/uxp/securityserver-ui/                 /usr/share/uxp/securityserver-ui/
COPY --from=bilder /tmp/identity-ui/usr/share/uxp/identity-provider-ui/     /usr/share/uxp/identity-provider-ui/

#ENTRYPOINT ["top", "-b"]
#ENTRYPOINT ["/bin/sh","nginx"] /usr/sbin/nginx
#RUN rm /bin/sh && rm /bin/bash
#USER nobody
ENTRYPOINT ["nginx", "-g", "daemon off;"]