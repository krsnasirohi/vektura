

Vektura is a simple school bus tracker. The system has a Arduino based hardware module and a cloud software component to display the bus location on a map.\

TBD HARDWARE ARCHITECTURE

Software Architecture

The project currently runs on AWS. The front end is HTML, CSS and JS and is hosted on S3 with a CloudFront based CDN in front. The backend is implemented using API Gateway, Lambda and DynamoDB. In this implementation all services are server-less, for easier maintenance, and lower costs.\

Software Installation

- Database
 - Create a database called "gpslogs" in DynamoDB with partition key "routeId" and sort key "time", both of type string. 

- Lambda
 - Implement two Lambda functions - one to write current GPS location and one function to fetch it. Use code provided for both functions. Select runtime as NodeJS 18.x. 

- API Gateway
 - Deploy an API with a GET method for resource "/getloc". The method request should have a required HTTP request header "routeId". The integration request should have a mapping template:  \{"routeId": "$input.params('routeId')"\}

 - In the same API implement another GET method for resource "/log". The method request should have 3 required URL query string parameters - lat, longitude and time. The integration request should have a mapping template: 

 - {
     "routeId": "$input.params('routeId')",
     "lat": "$input.params('lat')",
     "longitude": "$input.params('longitude')",
     "time": "$input.params('time')"
     }

 - The URL of your API deployment should be configured in search.js (line 2) in the front end app. 

- Front End
 - Configure API Gateway URL in line 3 and line 97 of script.js, this is the URL of your API deployment in API Gateway. Configure the routeId (this routeId is sent by the hardware module to the backend in line 3 and 97 of script.js. Also configure the API key for Google Maps (you will need a Google developer account) in line 26 of index.html.}