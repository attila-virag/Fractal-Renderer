using System;

namespace CalculatorInterface
{
  public struct ZoomData
  {
    public double X { get; }
    public double Y { get; }
    public double Scale { get; }


  }


  public interface ICalculatorInterface
  {
    double X { get; set; }
    double Y { get; set; }
    double Scale { get; set; }
    int Pixels { get; set; }

    //static Create();;// Create();
    void CreateDefault();

  }
}
