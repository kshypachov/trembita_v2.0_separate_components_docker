FROM ubuntu:22.04 AS bilder
ENV DEBIAN_FRONTEND=noninteractive

LABEL authors="Kirill Shypachov @kshypachov"
COPY uxp-securityserver-ui_1.4.6_all.deb /uxp-securityserver-ui_1.4.6_all.deb
RUN mkdir /tmp/ui
RUN dpkg-deb -X /uxp-securityserver-ui_1.4.6_all.deb /tmp/ui
RUN chmod 644 -R /tmp/ui

COPY uxp-securityserver-rest-api_1.22.7_all.deb /uxp-securityserver-rest-api_1.22.7_all.deb
RUN mkdir /tmp/rest-api
RUN dpkg-deb -X /uxp-securityserver-rest-api_1.22.7_all.deb /tmp/rest-api
RUN chmod 644 -R /tmp/rest-api

FROM ubuntu:22.04
RUN apt update && apt install -qq --no-install-recommends -y nginx && rm -rf /var/lib/apt/lists/* && apt clean

COPY default-uxp.conf /etc/nginx/sites-enabled/default-uxp
COPY --from=bilder /tmp/rest-api/etc/uxp/nginx/* /etc/nginx/conf.d/

RUN rm /etc/nginx/conf.d/default-uxp.conf
RUN rm /etc/nginx/sites-enabled/default

#/etc/nginx/conf.d/
#/etc/nginx/sites-enabled/default

ENTRYPOINT ["top", "-b"]