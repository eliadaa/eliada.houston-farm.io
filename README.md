open here: 
[# eliada.houston-farm.io](https://eliadaa.github.io/eliada.houston-farm.io/)


We have developed an innovative prototype of an IoT Smart Greenhouse, designed to revolutionize sustainable agriculture by incorporating advanced automation and environmental intelligence. Our Smart Greenhouse features an automatic irrigation system, artificial lighting, and retractable rooftops, all seamlessly integrated to optimize the growing environment.
Equipped with a comprehensive suite of sensors, the system monitors internal air temperature, humidity levels, soil moisture, and luminosity, ensuring precise, real-time data collection. What sets our greenhouse apart is its ability to utilize NASA’s climate databases, enabling it to dynamically respond to external environmental conditions. This integration allows the system to make informed adjustments for the best possible crop conditions while minimizing resource usage.
For example, if the internal soil moisture sensor detects low humidity, but the NASA data predicts rain in the area, the rooftops will retract, allowing natural precipitation to irrigate the crops. Conversely, in cases of high external heat or intense sunlight, the automatic irrigation system activates to maintain optimal soil moisture levels.
This cutting-edge solution is designed not only to improve yield and crop quality but also to do so in an environmentally conscious manner, reducing water consumption and enhancing energy efficiency. Our IoT Smart Greenhouse provides farmers with a powerful tool to sustainably manage their crops, leveraging NASA’s space-age data for down-to-earth agricultural needs.
Additionally, we have developed a user-friendly web application that provides farmers with a comprehensive platform to remotely monitor both internal and external conditions of the greenhouse. Through this web app, farmers can view real-time data on air temperature, humidity, soil moisture, and luminosity, as well as receive insights from NASA's climate datasets. This empowers them with full visibility and control, allowing them to make informed decisions and intervene when necessary.


→Web Application:
Our Web Application offers an interactive and user-friendly interface for the users to specify the location of their Greenhouse in order to set up the best environment for the crop while being ECO friendly and consuming the least resources possible. 
The Application has:
The Landing page includes a drop down list with different cities the user can choose from and a button to register the chosen option. Once pressed, the annual Temperature Mean of the specific location is sent to the Greenhouse in order to adjust the duration of the sprinklers’ watering loop and the rooftops’ position. The Temperature mean was computed using the NASA GISS Surface Temperature Analysis (v4) Data provided at link. The Temperature Mean is equal to the arithmetic mean of all the respective per month means taking into consideration the last 15 entries. 
The Second page where other than displaying the computed Temperature Mean for the selected city, we also display statistics of the On Site Data gathered by our System’s sensors in the form of a graphic. The following data were displayed: Temperature, Air Humidity, Luminosity, Soil Moisture and Water Level. 
!! THE TEMPERATURE MEAN OF THE CITIES FROM THE LAST 15 YEARS WAS COMPUTED MANUALLY AND HARDCODED INTO THE PROGRAM. FUTURE IMPROVEMENTS WOULD BE FOR THE TEMPERATURE MEAN TO BE AUTOMATICALLY COMPUTED FOR THE CITIES AND TO INCLUDE MORE CITIES (IN THIS CASE TIMISOARA AND CLUJ-NAPOCA WERE USED FOR THE PROTOTYPE) !!

→Use-Case Scenarios:
*The Water Pump will only be activated IF the Soil Humidity Sensor reveals Low Levels of Humidity in order to avoid flooding the crop.
The following Use-Case Scenarios were implemented (to be further developed in many more)  
Temperature Mean >= 11 Degrees Celsius
The Rooftops open ⇔ Servo Motor is set to 0 degrees  
The Irrigation System will be set to a longer duration loop ⇔ Submersible Water Pump will be activated with a higher delay (6000 or 6 seconds)
The Internal Heating Lights stop functioning ⇔ LED light turns OFF

Temperature Mean < 11 Degrees Celsius
The Rooftops close ⇔ Servo Motor is set to 180 degrees  
The Irrigation System will be set to a shorter duration loop ⇔ Submersible Water Pump will be activated with a smaller delay (2000 or 2 seconds)
The Internal Heating Lights start functioning ⇔ LED light turns ON
