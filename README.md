# DBLP Reader

It will download dblp from official dump (maintain required sometimes), parses by libxml2 using SAX.

Filter paper by some specific author names, and dump the result to json.


This project depends on [ArgCV](https://github.com/yuikns/argcv)


## Usage:

+ Download DBLP dump:

```
sh ./scripts/download_data.sh
```

+ Read dump, export "Jimeng Sun" to a dump named as 'publications.json' in current directory:

```
bazel run :paper_collecter -- \
  --in="$(pwd)/data/dblp.xml" \
  --name="Jimeng Sun" \
  --out="$(pwd)/data/publications.json"
```



