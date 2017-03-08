<?php
include ("jpgraph/jpgraph.php");	
include	("jpgraph/jpgraph_line.php");
include	("jpgraph/jpgraph_scatter.php");
include ("jpgraph/jpgraph_plotline.php");

function make_data_plot($plot_name, $x_data, $y_data, $title, 
		   $y_label, $h_line, $h_line_plus, $h_line_minus)
{
  
  $x_min = min($x_data);
  $x_max = max($x_data);
  $y_min = min($y_data);
  $y_max = max($y_data);
  
  $y_min = min([$y_min, ($h_line-2*$h_line_minus)]);
  $y_max = max([$y_max, ($h_line+2*$h_line_plus)]);
    
  $width = 600; $height = 400;
 
  $graph = new Graph($width,$height);
  $graph->SetScale("intlin", $y_min, $y_max, $x_min, $x_max);
  $graph->img->SetMargin(100, 50, 10, 80);  

  $graph->xgrid->Show(true);
  $graph->ygrid->Show(true);
  $graph->xaxis->Setcolor($_SESSION['textcolour']);
  $graph->yaxis->Setcolor($_SESSION['textcolour']);
  $graph->xaxis->SetTitleMargin(50);
  $graph->yaxis->SetTitleMargin(70);
  $graph->SetFrame(true,$_SESSION['bgcolour'],0);

  if ($title != "")
    {
      $graph->title->Set($title);
      $graph->title->SetColor($_SESSION['textcolour']);
      $graph->title->SetFont(FF_FONT1,FS_BOLD);	
    }

  $graph->SetFrame(true,$_SESSION['bgcolour'], 1);
  $graph->SetBackgroundGradient('darkblue','blue', GRAD_MIDHOR, BGRAD_PLOT);
  $graph->SetColor("darkblue");
  $graph->SetMarginColor($_SESSION['bgcolour']);
  $graph->xgrid->SetColor("black");
  $graph->ygrid->SetColor("black");

  $graph->xaxis->title->Set('Housing number');
  $graph->yaxis->title->Set($y_label);
  
  $scatterplot = new ScatterPlot($y_data, $x_data);
  $scatterplot->mark->SetType(MARK_FILLEDCIRCLE);
  $scatterplot->mark->SetFillColor("red");
  
  $graph->Add($scatterplot);
  
  
  if (!empty($h_line))
    {
      $target_line = new PlotLine(HORIZONTAL, $h_line, "green", 2);
      $graph->AddLine($target_line);
      $target_line_plus = new PlotLine(HORIZONTAL, $h_line+$h_line_plus, "green", 2);
      $target_line_plus->SetLineStyle('dotted');
      $graph->AddLine($target_line_plus);
      $target_line_minus = new PlotLine(HORIZONTAL,$h_line-$h_line_minus, "green", 2);
      $target_line_minus->SetLineStyle('dotted');
      $graph->AddLine($target_line_minus);
    }
  

  $graph->Stroke($plot_name);
  
  return $plot_name;    
}

?>
