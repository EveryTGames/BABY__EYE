using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;




[CreateAssetMenu(fileName = "bluetooth", menuName = "bluetooth/new bluetooth device")]
public class bluetootHandler : ScriptableObject
{
    public static bool IsConnected = false;
  


    static Dropdown menu;
  static  string selectedDevice;
   static string[] devices;
    static Text alarm;


    public static void makeAWifi()
    {
        BluetoothService.WritetoBluetooth("0" + "\n");
        //load the make a wifi page
        SceneManager.UnloadSceneAsync(2);

        SceneManager.LoadScene(6, LoadSceneMode.Additive);
        // BluetoothService.WritetoBluetooth("1");
    }


    public static void connectToWifi()
    {
        BluetoothService.WritetoBluetooth("1" + "\n");
        SceneManager.UnloadSceneAsync(2);

        SceneManager.LoadScene(4,LoadSceneMode.Additive);
        // BluetoothService.WritetoBluetooth("1");
    }
  static  bool  isok = false;
    public static void update(int currentScene)
    {
        if (IsConnected)
        {
            try
            {
                string datain = BluetoothService.ReadFromBluetooth();
                if (datain.Length >= 1)
                {
                    
                    
                    // BluetoothService.Toast("received : " + datain);//test this to see what we receive actually            
                    if ("1" == datain)
                    {
                        BluetoothService.Toast("wifi Set up was done correctly");
                        isok = true;
                       return;
                    }
                    if ("0" == datain)
                    {
                       wifiHandling.wifiConnected = false;
                        BluetoothService.Toast( "something went wrong try again please");
                    }
                    if (isok)
                    {

                        wifiHandling.init(datain);
                        
                        ip x = new ip();
                        x.Ip = datain;
                        string json = JsonUtility.ToJson(x);
                        File.WriteAllText("/storage/emulated/0/Android/data/com.ETGAMES.BABYEye/files/saves.txt", json);


                        SceneManager.UnloadSceneAsync(currentScene);
                        SceneManager.LoadScene(3,LoadSceneMode.Additive);
                    }


                }

            }
            catch (Exception e)
            {
                BluetoothService.Toast("Error in connection"+e.Message);
                    alarm.text = e.Message;
            }
        }
    }

    public static void GetDevicesButton()
    {
        
        GameObject dropdownObject = GameObject.Find("Dropdown (Legacy)");
        GameObject alarmObject = GameObject.Find("alarm message");
        if(dropdownObject == null ) 
        {
            BluetoothService.Toast("Error Occured");
        }
        if(alarmObject == null )
        {
            BluetoothService.Toast("Error 2 occuried");

        }
        menu = dropdownObject.GetComponent<Dropdown>();
        alarm = alarmObject.GetComponent<Text>();

        devices = BluetoothService.GetBluetoothDevices();

        menu.ClearOptions();
        if (devices != null && devices.Length > 0)
        {
            alarm.text = "found " + devices.Length + " devices";


            menu.AddOptions(devices.ToList());
        }
        else
        {
            alarm.text = "no devices found";
        }


    }





    public static  void StartButton()
    {
    
        if (!IsConnected)
        {
            if (devices != null && devices.Length > 0)
            {
                BluetoothService.CreateBluetoothObject();
                BluetoothService.Toast( "connecting, please wait...");
                selectedDevice = devices[menu.value];
                IsConnected = BluetoothService.StartBluetoothConnection(selectedDevice.ToString());
                BluetoothService.Toast(selectedDevice.ToString() + " status: " + IsConnected);
                
                if (IsConnected)
                {
                    SceneManager.UnloadSceneAsync(1);

                    SceneManager.LoadScene(2, LoadSceneMode.Additive);
                }
            }

        }
    }



    public static void StopButton()
    {
        if (IsConnected)
        {
            BluetoothService.StopBluetoothConnection();
        }
        Application.Quit();
    }


    public static bool sendSsidAndPassword(string ssid, string password)
    {
        if(IsConnected)
        {


        BluetoothService.WritetoBluetooth("0" + "\n");
        Thread.Sleep(500);

        BluetoothService.WritetoBluetooth(ssid + "\n");
        Thread.Sleep(500);


        BluetoothService.WritetoBluetooth(password + "\n");
            return true;
        }
        else
        {
            return false;
        }    



       


    }
}
