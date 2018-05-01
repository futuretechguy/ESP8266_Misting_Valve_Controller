#ifndef PAGE_DeviceControlPage_H
#define PAGE_DeviceControlPage_H

const char PAGE_DeviceControlPage[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Water Valve</title>

    <style id="style4538">
        .deviceimagegrid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            grid-template-rows: 20px 1fr 20px 20px 32px 20px 36px 36px 20px;
            grid-template-areas: "mtitle mtitle" "mdata mdata" "vlabel vdata" "flabel fdata" "blabel bdata" "schedule schedule" "timestart timestartbtn" "timeend timeendbtn" "footer footer";
            grid-gap: 8px;

        }

        .svg8 {
            height: 240;
            width: 240;
        }

        .pump {
            stroke-width: 0.99999994;
            fill: gray;
        }

        .drop {
            fill: gray;
        }

        /*.deviceimagegrid>div {
            border: violet 1px solid;

        }*/

        .mtitle {
            grid-area: mtitle;
            display: grid;
            grid-template-columns: 1fr 1f 1fr 1fr 1f;
            grid-template-areas: "humlbl humid subtitle templbl temp";

        }

        /*.mtitle>div{
            border: violet 1px solid;
        }*/

        .humid {
            grid-area: humid;
            display: grid;
            grid-template-columns: 40px 20px
        }

        /*.humid>div {
          border: violet 1px solid;
        }*/

        .subtitle {
            grid-area: subtitle;
        }

        .temp {
            grid-area: temp;
            display: grid;
            grid-template-columns: 40px 20px
        }

        .mdata {
            grid-area: mdata;
            background: rgb(191, 228, 245);
        }

        .vlabel {
            grid-area: vlabel;

        }

        .vdata {
            grid-area: vdata;
            display: grid;
            grid-template-columns: .5fr 10fr;
            padding-top: 1px;

        }

        .flabel {
            grid-area: flabel;

        }

        .fdata {
            grid-area: fdata;
            display: grid;
            grid-template-columns: .5fr 10fr;
            padding-top: 1px;

        }

        .blabel {
            grid-area: blabel;
            padding-top: 5px;
        }

        .bdata {
            grid-area: bdata;

        }

        .schedule {
            grid-area: schedule;
            background: rgb(191, 228, 245);
        }

        .timestart {
            grid-area: timestart;
        }

        .timeend {
            grid-area: timeend;
        }

        .timestartbtn {
            grid-area: timestartbtn;
            padding-top: 3px;
        }

        .timeendbtn {
            grid-area: timeendbtn;
            padding-top: 3px;
        }

        .footer {
            grid-area: footer;
        }



        .btn {
            background: #3498db;
            background-image: -webkit-linear-gradient(top, #3498db, #2980b9);
            background-image: -moz-linear-gradient(top, #3498db, #2980b9);
            background-image: -ms-linear-gradient(top, #3498db, #2980b9);
            background-image: -o-linear-gradient(top, #3498db, #2980b9);
            background-image: linear-gradient(to bottom, #3498db, #2980b9);
            -webkit-border-radius: 28;
            -moz-border-radius: 28;
            border-radius: 28px;
            font-family: Arial;
            color: #ffffff;
            font-size: 17px;
            padding: 3px 20px 3px 20px;
            text-decoration: none;
        }

        .btn:hover {
            background: #3cb0fd;
            background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
            background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
            background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
            background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
            background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
            text-decoration: none;
        }

        .onoffswitch {
            position: relative;
            width: 71px;
            /*-webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;*/
        }

        .onoffswitch-checkbox {
            display: none;
        }

        .onoffswitch-label {
            display: block;
            overflow: hidden;
            cursor: pointer;
            border: 2px solid #E3E3E3;
            border-radius: 36px;
        }

        .onoffswitch-inner {
            display: block;
            width: 200%;
            margin-left: -100%;
            transition: margin 0.3s ease-in 0s;
        }

        .onoffswitch-inner:before,
        .onoffswitch-inner:after {
            display: block;
            float: left;
            width: 50%;
            height: 25px;
            padding: 0;
            line-height: 25px;
            font-size: 16px;
            color: white;
            font-family: Trebuchet, Arial, sans-serif;
            font-weight: bold;
            box-sizing: border-box;
        }

        .onoffswitch-inner:before {
            content: "On";
            padding-left: 10px;
            background-color: #49E845;
            color: #FFFFFF;
        }

        .onoffswitch-inner:after {
            content: "Off";
            padding-right: 10px;
            background-color: #0D0C0C;
            color: #F5F2F2;
            text-align: right;
        }

        .onoffswitch-switch {
            display: block;
            width: 25px;
            margin: 0px;
            background: #FFFFFF;
            position: absolute;
            top: 0;
            bottom: 0;
            right: 42px;
            border: 2px solid #E3E3E3;
            border-radius: 36px;
            transition: all 0.3s ease-in 0s;
        }

        .onoffswitch-checkbox:checked+.onoffswitch-label .onoffswitch-inner {
            margin-left: 0;
        }

        .onoffswitch-checkbox:checked+.onoffswitch-label .onoffswitch-switch {
            right: 0px;
        }

        .inputdiv {
            padding-top: 1px;
        }

        .dtinput {
            padding: 3px;
            font-size: 16px;
            border-width: 2px;
            border-color: #CCCCCC;
            background-color: #FFFFFF;
            color: #000000;
            border-style: ridge;
            border-radius: 15px;
            /*box-shadow: 0px 0px 5px rgba(66, 66, 66, .75);
            /*text-shadow: 0px 0px 5px rgba(66, 66, 66, .75);*/
        }

        .css-input:focus {
            outline: none;
        }

        @media screen and (max-width: 736px) {
            .deviceimagegrid {
                display: grid;
                grid-template-columns: 1fr 120px 120px 1fr;
                grid-template-rows: 20px 1fr 20px 20px 32px 20px 36px 36px 20px;
                grid-template-areas: ". mtitle mtitle ." ". mdata mdata ." ". vlabel vdata ." ". flabel fdata ." ". blabel bdata ." ". schedule schedule. " ". timestart timestartbtn ." ". timeend timeendbtn ." ". footer footer .";
                grid-gap: 2px;

            }

            .svg8 {
                height: 220;
                width: 220;
            }


           .templbl {
                visibility: hidden;  
            }
            
            .templbl:before {
                content: 'T:';
                visibility:visible;
            }

            .humidlbl {
                visibility: hidden;
            }
            
            .humidlbl:before {
                content: 'H:';
                visibility:visible;
            }
        }
    </style>
</head>

<body>
    <script type="text/javascript">
        var OffTimeChangeFlag = 1;
        var OnTimeChangeFlag = 1;
        var MessageChangeFlag = 1;
        var StatusTimer;
        
        window.onload = function () {

            var ws = new WebSocket("ws://" + location.hostname + ":81/");  // location.
            var conn_status = document.querySelector("#Server_Status");
            
            ws.onopen = function (evt) {
                conn_status.innerHTML = "Status: Connected!"
            };

            ws.onmessage = function (evt) {
                var newMessage = document.createElement('p');
                newMessage.textContent = "Server: " + evt.data + "\r\n At:" + timenow();
                ProcessMessage(evt.data)
            };

            ws.onclose = function (evt) {
                alert("Connection closed");
                conn_status.innerHTML = "Status: Connection closed!"               
            };

            ws.onerror = function (error) {
                console.log('WebSocket Error ' + error);
                conn_status.innerHTML = "Status: an error ocured";
            };


            document.getElementById("Start_Set").onclick = function (evt) {
                evt.preventDefault();
                var message;
                //alert("start set")
                if (document.getElementById("Start_Time").value != "") {
                    message = "start@" + document.getElementById("Start_Time").value;
                } else {
                    message = "start@";
                }
                ws.send(message);
                OnTimeChangeFlag = 1;
                MessageChangeFlag = 1;

            };

            document.getElementById("End_Set").onclick = function (evt) {
                evt.preventDefault();
                var message;
                
                if (document.getElementById("End_Time").value != "") {
                    message = "stop@" + document.getElementById("End_Time").value;
                } else {
                    message = "stop@";
                }
                ws.send(message);
                OffTimeChangeFlag = 1;
                MessageChangeFlag = 1;
            };



            document.querySelector("#Flow_Switch").onchange = function (evt) {
                var message;
                var oDropImg = document.querySelector("#DropImg");
                var oPumpImg = document.querySelector("#PumpImg");

                if (document.getElementById("Flow_Switch").checked) {
                    message = "0";
                    oPumpImg.style.fill = "coral";
                    oDropImg.style.fill = "lightblue";

                } else {
                    message = "1";
                    oPumpImg.style.fill = "gray";
                    oDropImg.style.fill = "gray";
                };
                ws.send(message);
                MessageChangeFlag = 1;
                
            };

        };

        function timenow() {
            var now = new Date(),
                ampm = 'am',
                h = now.getHours(),
                m = now.getMinutes(),
                s = now.getSeconds();
            if (h < 10) h = '0' + h;
            if (m < 10) m = '0' + m;
            if (s < 10) s = '0' + s;
            return h + ':' + m
        }

        function ClearStatus() {         
            document.getElementById("Server_Status").innerText = "Status: Connected";
            clearTimeout(StatusTimer);
        }


        function tConvert (time) {
         // Check correct time format and split into components
         time = time.toString ().match (/^([01]\d|2[0-3])(:)([0-5]\d)(:[0-5]\d)?$/) || [time];

         if (time.length > 1) { // If time format correct
            time = time.slice (1);  // Remove full string match value
            time[5] = +time[0] < 12 ? 'AM' : 'PM'; // Set AM/PM
            time[0] = +time[0] % 12 || 12; // Adjust hours
         }
         return time.join (''); // return adjusted time or original string
         }

        function ProcessMessage(oMsgvalue) {
            obj = JSON && JSON.parse(oMsgvalue) || $.parseJSON(oMsgvalue);
            
            
            if (isNaN(obj.FlowRate) == false || obj.FlowRate != "") { 
                document.getElementById("Flow_Rate").innerText = obj.FlowRate;
                //alert(obj.FlowRate)
            }
             
            if (isNaN(obj.FlowVolume) == false || obj.FlowVolume != "") {
                document.getElementById("Flow_Vol").innerText = obj.FlowVolume;
            }

            if (isNaN(obj.Temperature) == false || obj.Temperature != "") {

                document.getElementById("Temperature").innerText = obj.Temperature;
            }
             
            if (isNaN(obj.Humidity) == false || obj.Humidity != "") {

                document.getElementById("Humidity").innerText = obj.Humidity;
            }
             
            if (isNaN(parseInt(obj.Start)) == false) {
                if (obj.Start != "00:00" && OnTimeChangeFlag == 1) {                  
                    document.getElementById("Start_Time").value = obj.Start;
                    OnTimeChangeFlag = 0;                                  
                }
            }

            if (isNaN(parseInt(obj.Stop)) == false) {
                if (obj.Stop != "00:00" && OffTimeChangeFlag == 1) {                   
                    if (OffTimeChangeFlag == 1) {
                        document.getElementById("End_Time").value = obj.Stop;
                        OffTimeChangeFlag = 0;                      
                    }
                }
            } 
            
            if (isNaN(obj.Valve) == false || obj.Valve != "") {
                var oDropImg = document.querySelector("#DropImg");
                var oPumpImg = document.querySelector("#PumpImg");   
                if  (obj.Valve == "On") {
                    document.getElementById("Flow_Switch").checked = true
                    oPumpImg.style.fill = "coral";
                    oDropImg.style.fill = "lightblue";
                   
                } else if (obj.Valve == "Off") {
                    document.getElementById("Flow_Switch").checked = false 
                    oPumpImg.style.fill = "gray";
                    oDropImg.style.fill = "gray";
                    
                }
            }
            if (isNaN(obj.Message) == false || obj.Message != "") { 
                if (MessageChangeFlag == 1) {
                  document.getElementById("Server_Status").innerText = obj.Message;
                  StatusTimer = setTimeout(ClearStatus, 15000);
                  MessageChangeFlag = 0;
               }
            }
        }

    </script>


    <div align="center" class="deviceimagegrid">
        <div class="mtitle">
            <div align="right" class="humidlbl">Humidity:</div>
            <div class="humid">
                <div align="left" id=Humidity class="humidval"></div>
                <div align="left" class="humidunit">%</div>
            </div>
            <div Class "subtitle">
                <b>Misting System</b>
            </div>
            <div align="right" class="templbl">Temperature:</div>
            <div class="temp">
                <div id="Temperature" align="left" class="tempval"></div>
                <div align="left" class="tempunit">°F</div>
            </div>
        </div>
        <div class="mdata">
            <svg id="svg8" version="1.1" viewBox="0 0 54 54" height="240.0000" width="240.0000">

                <g style="display:inline" id="layer1">
                    <path style="display:inline;opacity:1;fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:0.13282089;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1"
                        d="m 13.43237,37.483552 c 10e-7,1.286859 -1.243808,2.330063 -2.778122,2.330063 -1.5343235,0 -2.7781321,-1.043206 -2.7781313,-2.330063 0,-1.286858 1.5327777,-6.136604 3.0670983,-6.136604 1.534315,0 2.489155,4.849746 2.489155,6.136604 z"
                        id="DropImg" inkscape:connector-curvature="0" sodipodi:nodetypes="ssscs" inkscape:label="WaterDrop" class="drop"
                    />
                    <path style="stroke-width:0.99999994" d="m 22.569664,53.255148 c -6.27245,-1.137334 -12.325351,-4.699823 -16.1635308,-9.513176 -2.07299,-2.599685 -4.32791,-7.112721 -5.09801,-10.203273 -3.69705,-14.836916 6.14966,-29.7496984 21.4063308,-32.4198019 9.09298,-1.59138003 19.0177,2.2103668 24.84371,9.5165909 2.091,2.622275 4.32227,7.102611 5.14724,10.335573 0.86902,3.405519 0.86902,9.029829 0,12.435348 -0.82497,3.232962 -3.05624,7.713298 -5.14724,10.335563 -5.82903,7.310038 -15.93035,11.155637 -24.9885,9.513176 z m 6.31898,-7.842855 c 1.80749,-0.38286 3.27084,-1.129124 3.80169,-1.938761 0.3976,-0.606397 0.3595,-0.816954 -0.69363,-3.833036 -0.61289,-1.755261 -1.21746,-3.191392 -1.3435,-3.191392 -0.12603,0 -0.46744,-0.23812 -0.75868,-0.529167 -0.47874,-0.478407 -0.52952,-0.881937 -0.52952,-4.207646 v -3.678482 l 0.92841,-0.753264 c 0.87388,-0.709017 0.95943,-0.924244 1.45627,-3.663669 0.68,-3.749319 0.64582,-3.666009 1.3321,-3.247122 0.90472,0.552217 1.31369,-0.305937 1.31369,-2.756545 0,-2.125698 0.31612,-2.661455 0.80781,-1.369081 0.14584,0.383347 0.50478,2.355805 0.79762,4.383233 0.88941,6.157607 1.556,7.771228 5.34456,12.937738 1.48585,2.026268 1.78715,2.611565 1.78715,3.471709 0,1.355904 0.40002,2.058111 1.17241,2.058111 0.74969,0 0.94567,-0.43237 0.94567,-2.086338 0,-1.524594 -0.84011,-3.298672 -2.52021,-5.32195 -2.63178,-3.169352 -3.61995,-5.69659 -4.08644,-10.450993 -0.57074,-5.816907 -1.14141,-7.538445 -3.26037,-9.835693 -2.01429,-2.1837574 -4.6023,-2.9163841 -7.41535,-2.0991574 -2.25615,0.6554367 -2.29859,0.7377964 -2.12029,4.1153754 l 0.15284,2.895125 -0.6346,0.118517 c -0.34903,0.0652 -1.34945,0.24999 -2.22316,0.41069 -2.2392,0.411857 -2.9351,0.994874 -2.82937,2.370455 0.094,1.223004 0.10161,1.218914 -2.73063,1.470271 -4.39025,0.38964 -7.9613008,2.766025 -8.5044608,5.65938 -0.15602,0.831074 -0.0786,1.029094 0.65994,1.688561 0.6526598,0.582744 1.0960808,0.747064 2.0160208,0.747064 1.00359,0 1.2669,-0.11838 1.76712,-0.794494 0.97315,-1.315384 2.47906,-1.869631 5.24502,-1.930441 2.18722,-0.0481 2.46209,9e-4 2.86817,0.511157 0.24703,0.31039 0.7172,0.701547 1.04484,0.869217 0.57854,0.296067 0.59573,0.421597 0.59573,4.354093 0,4.039099 -0.002,4.049979 -0.64791,4.344246 -0.91503,0.416627 -1.31949,1.162624 -2.1539,3.972679 -0.55319,1.862948 -0.67151,2.633005 -0.4857,3.161002 0.63707,1.810278 5.24564,2.922782 8.90066,2.148608 z m -1.57674,-29.112295 c -0.36101,-0.36078 -0.44871,-3.066555 -0.11632,-3.589332 0.2646,-0.416177 2.29624,-1.133461 3.19557,-1.128211 1.40932,0.008 1.48518,0.307117 0.57828,2.278465 -0.46012,1.000174 -0.93213,1.997091 -1.0489,2.215368 -0.2689,0.502647 -2.16664,0.665397 -2.60863,0.22371 z m -14.8328,22.324551 c 0.94327,-0.942634 0.86716,-2.106608 -0.30227,-4.622823 -0.59177,-1.273301 -1.16691,-2.315095 -1.27811,-2.315095 -0.394231,0 -2.5170108,3.850796 -2.6253608,4.762473 -0.2105,1.771468 1.07233,3.049022 2.8072608,2.795722 0.50096,-0.0731 1.13028,-0.352257 1.39848,-0.620277 z"
                        id="PumpImg" class="pump" />
                </g>
            </svg>
        </div>
        <div align="right" class="vlabel">Flow Volume:</div>
        
        <div class="vdata">
            <div id="Flow_Vol" align="left" class="voldata">0</div>
            <div align="left" id="Vol_unit" class="volunit">L</div>
        </div>
        
        <div align="right" class="flabel">Flow Rate:</div>
        
        <div class="fdata">
            <div id="Flow_Rate" align="left" class="flowdata">0</div>
            <div align="left" id="Flow_Unit" class="flowunit">L/H</div>
        </div>
        
        <div align="right" class="blabel">Flow Valve:</div>
        <div align="left" class="bdata">
            <div class=flipswitch>
                <div class="onoffswitch">
                    <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="Flow_Switch">
                    <label class="onoffswitch-label" for="Flow_Switch">
                        <span class="onoffswitch-inner"></span>
                        <span class="onoffswitch-switch"></span>
                    </label>
                </div>
            </div>
        </div>
        <div class="schedule">Schedule:</div>
        <div align="right" class="timestart">
            <input class="dtinput" size="8" type="time" id="Start_Time">
        </div>
        <div align="left" class="timestartbtn">
            <button class="btn" type="button" id="Start_Set">Set</button>
        </div>

        <div align="right" class="timeend">
            <input class="dtinput" size="8" type="time" id="End_Time">
        </div>
        <div align="left" class="timeendbtn">
            <button class="btn" type="button" id="End_Set">Set</button>
        </div>
        <div id="Server_Status" class="footer">Status: Connected!</div>
    </div>
</body>
</html>
)=====";
#endif