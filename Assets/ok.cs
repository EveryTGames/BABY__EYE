using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;

using UnityEngine;
using UnityEngine.UI;

public class ok : MonoBehaviour
{
    public InputField input;
    public Text alarm;
    public Text main;
    public void sene()
    {
        
        if (input.text != null && input.text.Length > 0)
        {

            wifiHandling.sendData(input.text);
            Debug.Log("sent" + input.text);
        }
    }
    public MyNetworkListener listener;
    AudioSource cry;
    AudioSource movemen;
    private void Start()
    {
        cry = GetComponent<AudioSource>();
        movemen = GetComponent<AudioSource>();

        try
        {
            listener = new MyNetworkListener(wifiHandling.client);
            //wifiHandling.start(listener);
            listener.DataReceived += onDataReceived;
            BluetoothService.Toast("started listeneing" + wifiHandling.ip);


            alarm = GameObject.Find("alarm message").GetComponent<Text>();

        }
        catch (Exception e) { BluetoothService.Toast(e.Message); }
    }
    bool available = false;
    string result;
    float time;
    float availableTime;
                int lastDet;
    private void Update()
    {


        if (available)
        {
            alarm.text = "";
            availableTime = Time.time;
            available = false;

            try
            {

            int mic = (int)ra.Find(item => item.Item1 == "mic").Item2;
            int mDetect = (int)ra.Find(item => item.Item1 == "moved").Item2;
            int distance = (int)ra.Find(item => item.Item1 == "distance").Item2;

            switch(mDetect)
                {
                    case 0:
                        lastDet = 0;

                        break;
                    case 1:
                        if(lastDet == 0)
                        {
                            //play the notification sound, make it not loop
                            movemen.Play(); 
                            alarm.text += "\n motion detected";
                        }
                        lastDet = 1;
                        

                        break;

                }
                switch(distance)
                {
                    case 0:
                        
                        break;
                    case 1:
                      
                            alarm.text += "\n baby is up";
                        
                       

                        break;

                }
            switch (mic)
            {
                case 0:
                    alarm.text += "\n there is some noise";
                    cry.Stop();
                    break;
                case 1:
                    alarm.text += "\n baby is crying";
                    cry.Play();
                    break;
                case 2:
                    alarm.text += "\n there is some noise similar to crying";
                    cry.Stop();

                    break;
                case 4:
                    alarm.text += "\n it is quit";
                    cry.Stop();


                    break;

            }
            }
            catch(Exception e)
            {
                Debug.Log("finally");
            }
            try
            {

                main.text = "";
                foreach (var c in ra)
                {
                    main.text += c.Item1 + ": " + c.Item2 + "\n";

                }
            }
            catch (Exception e)
            {
                Debug.Log("in update");
                alarm.text = e.Message + " in update";

            }


        }
        else if (Time.time - time >= 8.0f)
        {
            time = Time.time;
          //  alarm.text = result;
            wifiHandling.sendData("1");
            if(Time.time -  availableTime>= 20f)
            {
                alarm.text = "it seems the device lost connection, please connect to the same network or resetup it using bluetooth and if didnt work, then reset the ESP32";
            }
        }


    }

    public void quit()
    {
        wifiHandling.stop(listener);
        bluetootHandler.StopButton();
    }
    private void OnDestroy()
    {
        wifiHandling.stop(listener);

    }
    List<(string, float)> ra = new List<(string, float)>();

    enum receiveState
    {
        Ip,
        data,
        wait
    }
    receiveState state = receiveState.data;
    private void onDataReceived(UdpReceiveResult data)
    {
        BluetoothService.Toast("inreceiveeee");
        try
        {
            switch (state)
            {
                case receiveState.wait:
                    BluetoothService.Toast("wowwww");
                    break;
            }
        }
        catch (Exception e1)
        {
            Debug.Log("r u ediot");
            BluetoothService.Toast(data.Buffer.Length + " ddssssdd " + e1.Message);
        }

        if (data.Buffer != null && data.Buffer.Length > 0)
        {
            try
            {
                switch (state)
                {
                    case receiveState.data:
                        BluetoothService.Toast("in data");
                        break;
                }
            }
            catch (Exception e1)
            {
                BluetoothService.Toast(data.Buffer.Length + " dd " + e1.Message);
            }
            try
            {

                switch (state)
                {
                    case receiveState.wait:

                        try
                        {

                            BluetoothService.Toast("in wait");
                            result = Encoding.UTF8.GetString(data.Buffer);
                            if (result.StartsWith("\n"))
                            {
                                result = result.Substring(1);
                            }
                        if (result.EndsWith("\n"))
                        {
                            result = result.TrimEnd();
                        }
                        if (result == "ip")
                            {
                                state = receiveState.Ip;

                            }
                            else if (result == "data")
                            {
                                state = receiveState.data;
                                wifiHandling.wifiConnected = true;
                                BluetoothService.Toast("received data word");
                            }
                            else
                            {
                                BluetoothService.Toast("invalid message" + result);
                            }
                        }
                        catch(Exception e2) {
                            
                            BluetoothService.Toast(result);
                        }
                        break;
                    case receiveState.data:

                        try
                        {

                        BluetoothService.Toast("in data");
                            result = Encoding.UTF8.GetString(data.Buffer);
                       // Debug.Log(result);
                           // alarm.text = "";
                            if (result.StartsWith("\n"))
                            {
                                result = result.Substring(1);
                            }
                            if (result.EndsWith("\n"))
                            {
                                result = result.TrimEnd();
                                BluetoothService.Toast("ended");    
                            }
                            // alarm.text = result ;
                            string[] x = result.Split("\n");
                        BluetoothService.Toast("in here");

                        foreach (string s in x)
                            {
                                if (s != "" && s != null && s != " " && s != "  ")
                                {

                                    string[] y = s.Split(" ");
                                    bool found = false;


                                    for (int i = 0; i < ra.Count; i++)
                                    {
                                        if (ra[i].Item1 == y[0])
                                        {
                                            found = true;
                                            ra[i] = (y[0], float.Parse(y[1]));

                                        }

                                    }
                                    if (!found)
                                    {

                                        ra.Add((y[0], float.Parse(y[1])));
                                    }
                                }
                                else { Debug.Log("empty"); }

                            }
                        Debug.Log("is avalable");

                        available = true;
                        }
                        catch (Exception e)
                        {
                        BluetoothService.Toast(e.Message);
                            //main.text = e.Message + "in reveiving";

                        }


                        // Debug.Log("im in the event function" + result);
                        // BluetoothService.Toast("woowow" + result);

                        break;

                    case receiveState.Ip:


                        break;
                }


            }
            catch(Exception w)
            {
                BluetoothService.Toast(w.Message + "in ree");
            }

            
           

        }
    }
}

[SerializeField]
public class ip
{
    public string Ip;
}