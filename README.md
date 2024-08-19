import org.yaml.snakeyaml.Yaml

def data = """
      name: A
      type: X
      version: 12
      name: B
      type: Y
      version: 13
      name: C
      type: Z
      version: 14
"""

def lines = data.trim().split("\n")
def ret = []
def map = [:]

lines.each { line ->
    def (key, value) = line.split(":").collect { it.trim() }
    if (key == 'name' && map) {
        ret << map
        map = [:]
    }
    map[key] = value
}
ret << map // Add the last map

println ret


OnePageDemo
https://ftp.riken.jp/Linux/centos/7/isos/x86_64/
Objective: Do a Demo just one page.
           and then understand the basic feature.
```     
///////////
#Jenkinsfile for Pipeline
node('master'){
   stage('stage1'){
       sh '/usr/local/bin/oc login https://127.0.0.1:8443 -udeveloper -pa --insecure-skip-tls-verify'
       sh '/usr/local/bin/oc get route ' 
       def commit = sh (returnStdout: true, script: '/usr/local/bin/oc get route | grep  "tomcat"').split()
       def myurl = "account-management-api-web/healthcheck"
       echo "http://${commit[1]}/${myurl}"
    }
}

///////////
{
    "extends": "recommended",
    "rules": {
      "NoDef": "off",
      "UnnecessaryGString": "off"
    }
  }
when {
                expression { return !env.Myvar.contains('INIT') && env.Yourvar.contains('OK') }
            }
```
