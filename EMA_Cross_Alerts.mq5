//+------------------------------------------------------------------+
//|                                               EMA_Cross_Alerts.mq5
//|  Two EMAs (9 & 15). On cross -> popup, sound, push notification + arrows
//+------------------------------------------------------------------+
#property copyright "Generated"
#property version   "1.02"
#property indicator_chart_window
#property indicator_buffers 4
#property indicator_plots   4

// --- plot 1 (fast EMA)
#property indicator_label1  "EMA Fast"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrDodgerBlue
#property indicator_style1  STYLE_SOLID
#property indicator_width1  2

// --- plot 2 (slow EMA)
#property indicator_label2  "EMA Slow"
#property indicator_type2   DRAW_LINE
#property indicator_color2  clrOrangeRed
#property indicator_style2  STYLE_SOLID
#property indicator_width2  2

// --- plot 3 (buy arrows)
#property indicator_label3  "Buy Signal"
#property indicator_type3   DRAW_ARROW
#property indicator_color3  clrLime
#property indicator_width3  3

// --- plot 4 (sell arrows)
#property indicator_label4  "Sell Signal"
#property indicator_type4   DRAW_ARROW
#property indicator_color4  clrRed
#property indicator_width4  3

//--- inputs
input int    FastEMA = 9;                 // Fast EMA period
input int    SlowEMA = 15;                // Slow EMA period
input double ArrowDistance = 0.5;         // Arrow distance from bar (as % of bar range, 0.5 = 50%)
input bool   EnablePopup = true;          // Show popup Alert()
input bool   EnableSound = true;          // Play a sound
input string SoundFile = "alert.wav";     // Sound filename (in /Sounds/)
input bool   EnablePush = true;           // Send push notification (MetaQuotes ID must be configured)
input bool   AlertOncePerBar = true;      // Avoid repeating alerts within same bar

//--- indicator buffers (plotted on chart)
double bufFast[];
double bufSlow[];
double bufBuyArrow[];
double bufSellArrow[];

//--- handles
int handleFast = INVALID_HANDLE;
int handleSlow = INVALID_HANDLE;

//--- internal
datetime lastAlertBarTime = 0;

//+------------------------------------------------------------------+
//| Initialization                                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
   // basic validation
   if(FastEMA <= 0 || SlowEMA <= 0 || FastEMA == SlowEMA)
     {
      Print("EMA_Cross_Alerts: invalid EMA settings. Fast and Slow must be >0 and different.");
      return(INIT_FAILED);
     }

   // create EMA handles (MQL5 style: 6 parameters)
   handleFast = iMA(_Symbol, PERIOD_CURRENT, FastEMA, 0, MODE_EMA, PRICE_CLOSE);
   handleSlow = iMA(_Symbol, PERIOD_CURRENT, SlowEMA, 0, MODE_EMA, PRICE_CLOSE);

   if(handleFast == INVALID_HANDLE || handleSlow == INVALID_HANDLE)
     {
      PrintFormat("EMA_Cross_Alerts: failed to create MA handles (fast=%d slow=%d).", handleFast, handleSlow);
      // cleanup if one created
      if(handleFast != INVALID_HANDLE) IndicatorRelease(handleFast);
      if(handleSlow != INVALID_HANDLE) IndicatorRelease(handleSlow);
      return(INIT_FAILED);
     }

   // set buffers for plotting
   SetIndexBuffer(0, bufFast, INDICATOR_DATA);
   SetIndexBuffer(1, bufSlow, INDICATOR_DATA);
   SetIndexBuffer(2, bufBuyArrow, INDICATOR_DATA);
   SetIndexBuffer(3, bufSellArrow, INDICATOR_DATA);

   PlotIndexSetString(0, PLOT_LABEL, "EMA " + IntegerToString(FastEMA));
   PlotIndexSetString(1, PLOT_LABEL, "EMA " + IntegerToString(SlowEMA));
   PlotIndexSetString(2, PLOT_LABEL, "Buy Signal");
   PlotIndexSetString(3, PLOT_LABEL, "Sell Signal");

   // Set arrow codes (233 = solid up arrow, 234 = solid down arrow)
   PlotIndexSetInteger(2, PLOT_ARROW, 233);  // Solid up arrow for buy signals
   PlotIndexSetInteger(3, PLOT_ARROW, 234);  // Solid down arrow for sell signals

   // Initialize arrow buffers with empty values
   PlotIndexSetDouble(2, PLOT_EMPTY_VALUE, EMPTY_VALUE);
   PlotIndexSetDouble(3, PLOT_EMPTY_VALUE, EMPTY_VALUE);

   return(INIT_SUCCEEDED);
  }

//+------------------------------------------------------------------+
//| Deinitialization                                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   if(handleFast != INVALID_HANDLE) { IndicatorRelease(handleFast); handleFast = INVALID_HANDLE; }
   if(handleSlow != INVALID_HANDLE) { IndicatorRelease(handleSlow); handleSlow = INVALID_HANDLE; }
  }

//+------------------------------------------------------------------+
//| Calculate EMAs and check cross                                   |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
   // need at least 2 bars
   if(rates_total < 2)
      return(0);

   // Set arrays as NOT series (default indexing: 0 = oldest)
   ArraySetAsSeries(bufFast, false);
   ArraySetAsSeries(bufSlow, false);
   ArraySetAsSeries(bufBuyArrow, false);
   ArraySetAsSeries(bufSellArrow, false);

   // copy EMA values into our buffers (from indicator handles)
   int copiedFast = CopyBuffer(handleFast, 0, 0, rates_total, bufFast);
   int copiedSlow = CopyBuffer(handleSlow, 0, 0, rates_total, bufSlow);

   if(copiedFast <= 0 || copiedSlow <= 0)
     {
      PrintFormat("EMA_Cross_Alerts: CopyBuffer failed (fast=%d slow=%d).", copiedFast, copiedSlow);
      return(rates_total);
     }

   // Determine starting position
   int start = 1;
   if(prev_calculated > 1)
      start = rates_total - 2;  // Only recalculate last 2 bars on updates
   
   // Loop through bars to check for crossovers
   for(int i = start; i < rates_total; i++)
     {
      // Initialize arrows to EMPTY_VALUE
      bufBuyArrow[rates_total - 1 - i] = EMPTY_VALUE;
      bufSellArrow[rates_total - 1 - i] = EMPTY_VALUE;
      
      if(i == 0) continue; // Need previous bar for comparison
      
      // Get current and previous EMA values (non-series indexing)
      double fast_curr = bufFast[rates_total - 1 - i];
      double slow_curr = bufSlow[rates_total - 1 - i];
      double fast_prev = bufFast[rates_total - i];
      double slow_prev = bufSlow[rates_total - i];

      // CORRECTED: crossUp means fast crosses ABOVE slow (BUY), crossDown means fast crosses BELOW slow (SELL)
      bool crossUp   = (fast_prev < slow_prev) && (fast_curr > slow_curr);  // BUY: fast was below, now above
      bool crossDown = (fast_prev > slow_prev) && (fast_curr < slow_curr);  // SELL: fast was above, now below

      // Calculate arrow offset distance (adjustable based on price range)
      double barRange = high[rates_total - 1 - i] - low[rates_total - 1 - i];
      double arrowOffset = barRange * ArrowDistance;  // Use input parameter for distance
      
      // Set minimum offset to ensure visibility
      double minOffset = 20 * _Point;
      if(arrowOffset < minOffset) 
         arrowOffset = minOffset;

      if(crossDown)  // SELL signal - show red DOWN arrow
        {
         // Place red DOWN arrow ABOVE the high of the bar with offset
         bufSellArrow[rates_total - 1 - i] = high[rates_total - 1 - i] + arrowOffset;
         
         // Alert only on most recent bar
         if(i == rates_total - 1 && (!AlertOncePerBar || time[rates_total - 1] != lastAlertBarTime))
           {
            string txt = "EMA CROSS SELL: " + _Symbol + " | " + IntegerToString(FastEMA) + "/" + IntegerToString(SlowEMA)
                        + " | Price: " + DoubleToString(close[rates_total - 1], _Digits);

            if(EnablePopup) Alert(txt);
            if(EnableSound) PlaySound(SoundFile);
            if(EnablePush) SendNotification(txt);

            lastAlertBarTime = time[rates_total - 1];
            Print("EMA_Cross_Alerts: ", txt);
           }
        }
      else if(crossUp)  // BUY signal - show green UP arrow
        {
         // Place green UP arrow BELOW the low of the bar with offset
         bufBuyArrow[rates_total - 1 - i] = low[rates_total - 1 - i] - arrowOffset;
         
         // Alert only on most recent bar
         if(i == rates_total - 1 && (!AlertOncePerBar || time[rates_total - 1] != lastAlertBarTime))
           {
            string txt = "EMA CROSS BUY: " + _Symbol + " | " + IntegerToString(FastEMA) + "/" + IntegerToString(SlowEMA)
                        + " | Price: " + DoubleToString(close[rates_total - 1], _Digits);

            if(EnablePopup) Alert(txt);
            if(EnableSound) PlaySound(SoundFile);
            if(EnablePush) SendNotification(txt);

            lastAlertBarTime = time[rates_total - 1];
            Print("EMA_Cross_Alerts: ", txt);
           }
        }
     }

   return(rates_total);
  }
//+------------------------------------------------------------------+
