# DBLP Reader

It will download dblp from official dump (maintain required sometimes), parses by libxml2 using SAX.

Filter paper by some specific author names, and dump the result to json.


This project is depends on [ArgCV](https://github.com/yuikns/argcv)

You should clone the  [ArgCV](https://github.com/yuikns/argcv) first, and pleace this project in app foleders


## Usage:

+ Download DBLP dump:

```
sh ./app/dblp_reader/scripts/download_data.sh
```

+ Read dump, export "Jimeng Sun" to a dump named as 'publications.json' in current directory:

```
bazel run //app/dblp_reader:paper_collecter -- \
  --in="$(pwd)/app/dblp_reader/data/dblp.xml" \
  --name="Jimeng Sun" \
  --out="$(pwd)/publications.json"
```



