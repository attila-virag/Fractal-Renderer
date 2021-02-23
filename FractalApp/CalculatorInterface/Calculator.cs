using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;     // DLL support

namespace CalculatorInterface
{
  public class Calculator : ICalculatorInterface
  {
    private const string installDir = "F:\\FractalApp\\bin\\";
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool CreateFractalEngine(ref IntPtr zoomPtr, ref IntPtr colorPtr, ref IntPtr normPtr, ref IntPtr algoPtr);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Set_X_Value(IntPtr zoomPtr, ref double x_value);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Set_Y_Value(IntPtr zoomPtr, ref double y_value);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Set_Zoom(IntPtr zoomPtr, ref double zoom);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Set_Pixels(IntPtr zoomPtr, ref int pixels);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Get_X_Value(IntPtr zoomPtr, ref double x_value);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Get_Y_Value(IntPtr zoomPtr, ref double y_value);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Get_Zoom(IntPtr zoomPtr, ref double zoom);
    [DllImport(installDir + "FractalApp.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Get_Pixels(IntPtr zoomPtr, ref int pixels);


    public double X { get { double x_value = 0; Get_X_Value(zoomPtr, ref x_value); return x_value; } set { Set_X_Value( zoomPtr, ref value); } }
    public double Y { get { double y_value = 0; Get_Y_Value(zoomPtr, ref y_value); return y_value; } set { Set_Y_Value( zoomPtr, ref value); } }
    public double Scale { get { double scale = 0; Get_Zoom(zoomPtr, ref scale); return scale; } set { Set_Zoom(zoomPtr, ref value); } }
    public int Pixels { get { int pixels = 0; Get_Pixels(zoomPtr, ref pixels); return pixels; } set { Set_Pixels( zoomPtr, ref value); } }

    private IntPtr zoomPtr = IntPtr.Zero;
    private IntPtr palettePtr = IntPtr.Zero;
    private IntPtr normPtr = IntPtr.Zero;
    private IntPtr algoPtr = IntPtr.Zero;

    public Calculator()
    {
      Initialize();
    }

    private void Initialize()
    {
      CreateFractalEngine(ref zoomPtr,ref palettePtr,ref normPtr,ref algoPtr);
    }
  }
}
