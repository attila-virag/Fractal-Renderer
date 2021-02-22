using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Navigator
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {

    //string imagePath = "F:\\test\\testMandelbrotpicture.bmp";
    Uri uri = new System.Uri("F:\\test\\testMandelbrotpicture.bmp");

    private double display_x, display_y, display_scale;
    private double x, y, scale;

    private void SetCoordinates()
    {
      x = display_x;
      y = display_y;
      scale = display_scale;

      // should also call generate preview
    }

    public double Display_x   // property
    {
      get { return display_x; }   // get method
      set
      {
        if (value != display_x)
        {
          display_x = value;
          x_text.Text = Display_x.ToString("R");
        }
      }
    }

    public double Display_y  // property
    {
      get { return display_y; }   // get method
      set
      {
        if (value != display_y)
        {
          display_y = value;
          y_text.Text = Display_y.ToString("R");
        }
      }
    }

    public double Display_scale   // property
    {
      get { return display_scale; }   // get method
      set
      {
        if (value != display_scale)
        {
          display_scale = value;
          scale_text.Text = Display_scale.ToString("R");
          zoom_text.Text = Display_zoom.ToString("R");
        }
      }
    }

    public double Display_zoom   // property
    {
      get { return 1/display_scale; }   // get method
    }

    public MainWindow()
    {
      InitializeComponent();
      defaultImage.Source = new BitmapImage(uri);
      Display_x = 1.1;
      Display_y = -.5;
      Display_scale = 1;
      SetCoordinates();
    }

    // Plus button
    private void Click_Plus(object sender, RoutedEventArgs e)
    {
      //string message = "You have clicked +";
      //MessageBox.Show(message);
      Display_scale = Display_scale-Display_scale*0.1;
    }

    // minus button
    private void Click_Minus(object sender, RoutedEventArgs e)
    {
      //string message = "You have clicked -";
      //MessageBox.Show(message);
      Display_scale = Display_scale + Display_scale * 0.1;
    }

    private void Click_Up(object sender, RoutedEventArgs e)
    {
      Display_y = Display_y + Display_scale * 0.05;
    }

    private void Click_Down(object sender, RoutedEventArgs e)
    {
      Display_y = Display_y - Display_scale * 0.05;
    }

    private void Click_Left(object sender, RoutedEventArgs e)
    {
      Display_x = Display_x - Display_scale * 0.05;
    }

    private void Click_Right(object sender, RoutedEventArgs e)
    {
      Display_x = Display_x + Display_scale * 0.05;
    }

    private void Click_Revert(object sender, RoutedEventArgs e)
    {

      Display_x = x;
      Display_y = y;
      Display_scale = scale;
    }

    private void Click_Set(object sender, RoutedEventArgs e)
    {
      SetCoordinates();
      defaultImage.Source = new BitmapImage(uri);
      // calling worker thread to do a job
      // post a message on the worker thread queue
    }
  }
}
