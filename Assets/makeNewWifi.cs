using UnityEngine;
using UnityEngine.UI;

public class makeNewWifi : MonoBehaviour
{
    public Text alarm;
    public InputField ssid;
    public InputField password;


    private void Update()
    {
        try
        {
            alarm = GameObject.Find("alarm message").GetComponent<Text>();
        }
        catch { }
        bluetootHandler.update(6);
    }
    public void submet()
    {
        if ((ssid.text != null && ssid.text.Length > 0) && (password.text != null && password.text.Length > 0))
        {

            if (bluetootHandler.sendSsidAndPassword(ssid.text, password.text))
                alarm.text = "every thing sent";
            else
                alarm.text = "sending failed, device is not connected with bluetooth";
        }
        else
        {
            alarm.text = "cant take empty input";
        }
    }
}
