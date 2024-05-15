
using UnityEngine;
using UnityEngine.Android;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class BluetoothTest : MonoBehaviour
{


    public Text deviceName;
    public Text dataToSend;
    private bool IsConnected;
    public static string dataRecived = "";
    // Start is called before the first frame update

    public Text alarm;
    void Start()
    {
        try
        { 
        alarm = GameObject.Find("alarm message").GetComponent<Text>();
        }
        catch { }
#if UNITY_2020_2_OR_NEWER
#if UNITY_ANDROID
        if (!Permission.HasUserAuthorizedPermission(Permission.CoarseLocation)
          || !Permission.HasUserAuthorizedPermission(Permission.FineLocation)
          || !Permission.HasUserAuthorizedPermission("android.permission.BLUETOOTH_SCAN")
          || !Permission.HasUserAuthorizedPermission("android.permission.BLUETOOTH_ADVERTISE")
          || !Permission.HasUserAuthorizedPermission("android.permission.BLUETOOTH_CONNECT"))
            Permission.RequestUserPermissions(new string[] {
                        Permission.CoarseLocation,
                            Permission.FineLocation,
                            "android.permission.BLUETOOTH_SCAN",
                            "android.permission.BLUETOOTH_ADVERTISE",
                             "android.permission.BLUETOOTH_CONNECT"
                    });
#endif
#endif

        IsConnected = false;
        BluetoothService.CreateBluetoothObject();
      //  init();

    }



    //private static int localPort;

    //// prefs
    //private string IP;  // define in init
    //public int port;  // define in init

    //// "connection" things
    //IPEndPoint remoteEndPoint;
    //UdpClient client;

    //public void init()
    //{
    //    // Endpunkt definieren, von dem die Nachrichten gesendet werden.
    //    print("UDPSend.init()");

    //    // define
    //    IP = "192.168.1.10";
    //    port = 1234;

    //    // ----------------------------
    //    // Senden
    //    // ----------------------------
       


    //}
    
 


    bool wifiConnected = false;
    // Update is called once per frame
    void Update()
    {
        

    }


    //---------------------------------------
    //public Dropdown menu;
    //string selectedDevice;
    //string[] devices;
    //public void GetDevicesButton()
    //{
    //    devices = BluetoothService.GetBluetoothDevices();

    //    menu.ClearOptions();
    //    if (devices != null && devices.Length > 0)
    //    {
    //        alarm.text = "found " + devices.Length + " devices";


    //        menu.AddOptions(devices.ToList());
    //    }
    //    else
    //    {
    //        alarm.text = "no devices found";
    //    }


    //}



    //--------------------------------------------------------
    //public void StartButton()
    //{

     
    //    if (!IsConnected)
    //    {
    //        if (devices != null && devices.Length > 0)
    //        {
    //            alarm.text = "connecting, please wait...";
    //            selectedDevice = devices[menu.value];
    //            IsConnected = BluetoothService.StartBluetoothConnection(selectedDevice.ToString());
    //            BluetoothService.Toast(selectedDevice.ToString() + " status: " + IsConnected);
    //            alarm.text = "connection is " + IsConnected.ToString();
    //            if (IsConnected)
    //            {
    //                SceneManager.LoadScene(2, LoadSceneMode.Single);
    //            }
    //        }

    //    }
    //}




  
    //public void SendButton()
    //{
    //    if (IsConnected && (dataToSend.ToString() != "" || dataToSend.ToString() != null))
    //        BluetoothService.WritetoBluetooth(dataToSend.text.ToString());
    //    else
    //        BluetoothService.WritetoBluetooth("Not connected");
    //}
    public void newDevice()
    {

        SceneManager.UnloadSceneAsync(0);
        SceneManager.LoadScene(1, LoadSceneMode.Additive);
    }

    public void StopButton()
    {
    
        bluetootHandler.StopButton();

    }
}
