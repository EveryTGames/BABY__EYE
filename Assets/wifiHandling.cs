
using System.Net.Sockets;
using System.Net;
using UnityEngine;
using System.Text;


[CreateAssetMenu(fileName = "test", menuName = "testtest/rsrsr")]
public class wifiHandling : ScriptableObject
{
    public static bool wifiConnected = false;
    public static string ip;
    static IPEndPoint remoteEndPoint;
    public static UdpClient client;
    public static int port = 6859;
    public static void init(string _ip)
    {
        ip = _ip;
        remoteEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
        client = new UdpClient();
        byte[] data = Encoding.UTF8.GetBytes("connection completed");
        client.Send(data, data.Length, remoteEndPoint);
        wifiConnected = true;
        BluetoothService.Toast("connection completed sent");
    }

    public static void sendData(string message)
    {
        byte[] data = Encoding.UTF8.GetBytes(message);
        client.Send(data, data.Length, remoteEndPoint);

    }




    //public static void start(MyNetworkListener listener)
    //        {

    //                 listener.StartListening(client);

    //    } 

    public static void stop(MyNetworkListener listener)
        {

        
        listener.StopListening();
        wifiConnected = false;


    }

    



}
