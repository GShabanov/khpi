FROM ubuntu:latest


RUN apt-get update && \
    apt-get install -y curl && \
    rm -rf /var/lib/apt/lists/*


COPY ./webcheck.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/webcheck.sh

RUN echo 'start executin things'

CMD ["webcheck.sh"]
