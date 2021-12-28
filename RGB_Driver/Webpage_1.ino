
/*
  HTML Page, test here: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_intro
  Convert here: http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
*/




void Prep_webpage()
{

  String timeStr = timeClient.getFormattedTime();

    // !!! ELements that don't change !!!
  String page_head ="<!DOCTYPE html><html><head><style>td,th{ border: 1px solid #dddddd; text-align: left; padding: 8px;} tr:nth-child(even){background-color: #dddddd;}</style></head>";
  String body_start = "<body>";
  String body_table_start = String("<h2>") + DeviceHostName + String("</h2><h4>Updated: ") + timeStr + String("</h4><table>");
  String body_table_row00 ="<tr><th>#</th><th>Description</th><th>Value</th><th>Unit</th></tr>";
  String body_end = "</table></body></html>";

  // Start chaning here onwards.....

  String body_table_row01 =String("<tr><td>01</td><td>Intensity</td><td>")            + String(Intensity)              + String("</td><td>%</td></tr>");
  String body_table_row02 =String("<tr><td>02</td><td>Red Output 0-1023</td><td>")    + String(1023 - Red_Val)         + String("</td><td>-</td></tr>");
  String body_table_row03 =String("<tr><td>03</td><td>Green Output 0-1023</td><td>")  + String(1023 - Green_Val)       + String("</td><td>-</td></tr>");
  String body_table_row04 =String("<tr><td>04</td><td>Blue Output 0-1023</td><td>")   + String(1023 - Blue_Val)        + String("</td><td>-</td></tr>");
  
  

  webpage = page_head +
         body_start +
         body_table_start +
         body_table_row00 +
         body_table_row01 +
         body_table_row02 +
         body_table_row03 +
         body_table_row04 +
         body_end; 
         
}


/*

Source webpage:

<!DOCTYPE html>
<html>
<head>
<style>
td,th{
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}
tr:nth-child(even){
  background-color: #dddddd;
}
</style>
</head>
<body>

<h2>Device Name</h2>
<h4>Updated: Time</h4>
<table>
  <tr>
    <th>#</th>
    <th>Description</th>
    <th>Value</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>1</td>
    <td>Voltage</td>
    <td>231.1</td>
    <td>V</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Current</td>
    <td>0.21</td>
    <td>A</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Power</td>
    <td>3212</td>
    <td>W</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Energy</td>
    <td>241</td>
    <td>kWh</td>
  </tr>
  <tr>
    <td>5</td>
    <td>Frequeny</td>
    <td>50</td>
    <td>hz</td>
  </tr>
  
</table>

</body>
</html>





*/
