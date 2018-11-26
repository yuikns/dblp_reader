FROM yuikns/argcv as builder

ADD . /work

WORKDIR /work

RUN bazel build //:paper_collecter

# FROM scratch

# x509: failed to load system roots and no roots provided
# COPY --from=builder  /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/ca-certificates.crt

#COPY --from=builder /go/bin/sunlab-api /sunlab-api
# COPY --from=builder /work/bazel-bin/paper_collecter /paper_collecter

# EXPOSE 1234

ENTRYPOINT ["/work/bazel-bin/paper_collecter"]
