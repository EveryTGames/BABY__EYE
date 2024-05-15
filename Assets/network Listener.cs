using UnityEngine;
using System.ComponentModel;
using System;

using System.Net.Sockets;

public class MyNetworkListener 
{
    private BackgroundWorker worker;
    private UdpClient client; // Replace with your actual network client class

    public delegate void DataReceivedEventHandler( UdpReceiveResult data);
    public event DataReceivedEventHandler DataReceived;

    public  MyNetworkListener(UdpClient client)
    {
        this.client = client;
        worker = new BackgroundWorker();
        worker.DoWork += Worker_DoWork;
        worker.WorkerSupportsCancellation = true;
        worker.RunWorkerCompleted += Worker_RunWorkerCompleted;
        worker.RunWorkerAsync();
    }

    public void StopListening()
    {
        worker.CancelAsync();
    }

    private async void Worker_DoWork(object sender, DoWorkEventArgs e)
    {
        while (!worker.CancellationPending)
        {
                var resultTask = client.ReceiveAsync(); // Assuming client.ReceiveAsync is asynchronous
                var receivedData = await resultTask;
            try
            {
                if (DataReceived != null) // Check if there are subscribers
                {
                    DataReceived(receivedData); // Raise the event with data
                }
                // Raise the event with the received data (on the main thread)

            }
            catch (Exception ex)
            {
                Debug.Log(ex);
                // Handle any exceptions from network operations
                e.Result = ex; // Pass the exception to RunWorkerCompleted (optional)
                break;
            }
        }
    }

    private void Worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
    {
        // Optional: Handle any exceptions from DoWork (if any)
    }
}
