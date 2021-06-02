OnePageDemo

Objective: Do a Demo just one page.
           and then understand the basic feature.
```     
///////////////////////////////////////
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
```
