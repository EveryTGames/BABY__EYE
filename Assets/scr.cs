using System;
using System.IO;
using System.Security.Claims;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class scr : MonoBehaviour
{
    string ipp;
    Text alarm;
    // Start is called before the first frame update
    void Start()
    {
        SceneManager.LoadScene(5, LoadSceneMode.Additive);

        try
        {

        ip obj = new ip();
        obj = JsonUtility.FromJson<ip>(File.ReadAllText("/storage/emulated/0/Android/data/com.ETGAMES.BABYEye/files/saves.txt"));
        ipp = obj.Ip;
            BluetoothService.Toast("every thing fone at startup,the ip is" + ipp);
            alarm.text = "every thing fone at startup,the ip is" + ipp;
        }
        catch(Exception ex)
        {
            BluetoothService.Toast(ex.Message);
        }

    }

    public void Update()
    {
        try
        {

        alarm = GameObject.Find("alarm message").GetComponent<Text>();
        }catch(Exception ex)
        {
            BluetoothService.Toast(ex.Message);

        }

    }

    public void connectLast()
    {
        try
        {
         
        wifiHandling.init(ipp);
            alarm.text = ipp;
        SceneManager.UnloadSceneAsync(0);
        SceneManager.LoadScene(3, LoadSceneMode.Additive);
            BluetoothService.Toast("should done");
        }
        catch(Exception e) {
            
            alarm.text = e.Message;
        }

    }
}
