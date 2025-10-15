https://vektura.org/

Vektura is a simple school bus tracker. The system has a Arduino based hardware module and a cloud software component to display the bus location on a map.

Hardware Architecture
- Follow this video to build the hardware unit: https://www.youtube.com/watch?v=cWfhpgI53HI&t=69s
- Here is a list of parts I used for one unit: 
 - 1x Arduino Uno
 - 1x NEO6M 
 - 1x SIM800L
 - 1x Single 18650 mAh battery holder
 - 1x Double 18650 mAh battery holder
 - 1x Mini breadboard
 - 3x 18650 mAh batteries
 - Solderless breadboard jumper cables
 
Arduino code

Install the Arduino IDE on your computer and upload the code provided in the hardware folder of this repo into your Arduino Uno. You will need a CloudFront distribution in AWS in front of your API Gateway with the distrbution available on HTTP. It is easier to send GPS data from Arduino on HTTP rather than HTTPS, we need this CloudFront distribution for this reason. Configure the CloudFront distribution in line 141 in the Arduino code. Also enter a routeId in line 145. 

Software Architecture

The project currently runs on AWS. The front end is HTML, CSS and JS and is hosted on S3 with a CloudFront based CDN in front. The backend is implemented using API Gateway, Lambda and DynamoDB. In this implementation all services are server-less, for easier maintenance, and lower costs.

Software Installation

- Database
 - Create a database called "gpslogs" in DynamoDB with partition key "routeId" and sort key "time", both of type string. 

- Lambda
 - Implement two Lambda functions - one to write current GPS location and one function to fetch it. Use code provided for both functions. Select runtime as NodeJS 18.x. 

- API Gateway
 - Deploy an API with a GET method for resource "/getloc". The method request should have a required HTTP request header "routeId". The integration request should have a mapping template:  {"routeId": "$input.params('routeId')"}

 - In the same API implement another GET method for resource "/log". The method request should have 3 required URL query string parameters - lat, longitude and time. The integration request should have a mapping template: 

 - {
     "routeId": "$input.params('routeId')",
     "lat": "$input.params('lat')",
     "longitude": "$input.params('longitude')",
     "time": "$input.params('time')"
     }

 - The URL of your API deployment should be configured in search.js (line 2) in the front end app. 

- Front End
 - Configure API Gateway URL in line 3 and line 97 of script.js, this is the URL of your API deployment in API Gateway. Configure the routeId (this routeId is sent by the hardware module to the backend in line 3 and 97 of script.js. Also configure the API key for Google Maps (you will need a Google developer account) in line 26 of index.html.
