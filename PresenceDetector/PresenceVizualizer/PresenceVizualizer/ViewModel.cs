using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.Drawing;
using LiveChartsCore.Kernel.Sketches;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using LiveChartsCore.SkiaSharpView.Painting.Effects;
using SkiaSharp;
using System.Collections.ObjectModel;
using System.IO.Ports;

namespace PresenceVizualizer;

#pragma warning disable CA1416 // Validate platform compatibility

public class ViewModel
{
    private const int RangeInCm = 7800;
    private const int MinStepInCm = 1000;
    private const int TargetSize = 16;
    private static readonly string FrameHeader = "AAFF0300";
    private static readonly string FrameFooter = "55CC\n";

    private static readonly SKColor s_gray = new(195, 195, 195);
    private static readonly SKColor s_gray1 = new(160, 160, 160);
    private static readonly SKColor s_gray2 = new(90, 90, 90);
    private static readonly SKColor s_dark3 = new(60, 60, 60);

    private SerialPort _serial;

    public ObservablePoint[] Targets { get; } = [new ObservablePoint(), new ObservablePoint(), new ObservablePoint()];
    public ObservableCollection<ISeries> Series { get; }

    public ICartesianAxis[] XAxes { get; set; } = [
        new Axis
        {
            Name = "X axis",
            NamePaint = new SolidColorPaint(s_gray1),
            TextSize = 18,
            Padding = new Padding(5, 15, 5, 5),
            LabelsPaint = new SolidColorPaint(s_gray),
            SeparatorsPaint = new SolidColorPaint
            {
                Color = s_gray,
                StrokeThickness = 1,
                PathEffect = new DashEffect([3, 3])
            },
            ZeroPaint = new SolidColorPaint
            {
                Color = s_gray1,
                StrokeThickness = 2
            },
            TicksPaint = new SolidColorPaint
            {
                Color = s_gray,
                StrokeThickness = 1.5f
            },

//            CrosshairLabelsBackground = SKColors.DarkOrange.AsLvcColor(),
//            CrosshairLabelsPaint = new SolidColorPaint(SKColors.DarkRed),
//            CrosshairPaint = new SolidColorPaint(SKColors.DarkOrange, 1),
            MinStep = MinStepInCm,
            MinLimit = -RangeInCm,
            MaxLimit = RangeInCm
        }
    ];

    public ICartesianAxis[] YAxes { get; set; } = [
        new Axis
        {
                        Name = "Y axis",
            NamePaint = new SolidColorPaint(s_gray1),
            TextSize = 18,
            Padding = new Padding(5, 0, 15, 0),
            LabelsPaint = new SolidColorPaint(s_gray),
            SeparatorsPaint = new SolidColorPaint
            {
                Color = s_gray,
                StrokeThickness = 1,
                PathEffect = new DashEffect([3, 3])
            },
            ZeroPaint = new SolidColorPaint
            {
                Color = s_gray1,
                StrokeThickness = 2
            },
            TicksPaint = new SolidColorPaint
            {
                Color = s_gray,
                StrokeThickness = 1.5f
            },
            MinStep = MinStepInCm,
            MinLimit = 0, // -RangeInCm,
            MaxLimit = RangeInCm
        }
    ];

    public ViewModel()
    {
        _serial = new SerialPort("COM6", 115200);
        _serial.DataReceived += OnDataReceived;
        _serial.Open();
        Series = new ObservableCollection<ISeries>([
            new ScatterSeries<ObservablePoint>
                {
                    Values = Targets
                }
                ]);
    }

    private void OnDataReceived(object sender, SerialDataReceivedEventArgs e)
    {
        ParseFrame(_serial.ReadExisting());
    }

    private void ParseFrame(string input)
    {
        //System.Diagnostics.Debug.WriteLine(input);
        var values = input.Split('|', StringSplitOptions.TrimEntries);

        if (values.Length == 18)
        {
            for (int i = 0; i < Targets.Length; i++)
            {
                if (int.TryParse(values[i * 6], out var valid) && int.TryParse(values[i * 6 + 1], out var x) && int.TryParse(values[i * 6 + 2], out var y))
                {
                    if (valid == 1)
                    {
                        Targets[i].X = x;
                        Targets[i].Y = y;
                    }
                    else
                    {
                        Targets[i].X = null;
                        Targets[i].Y = null;
                    }
                }
            }
        }
    }
}
#pragma warning restore CA1416 // Validate platform compatibility
