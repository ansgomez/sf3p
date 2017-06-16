<?php

pcntl_signal(SIGPIPE, function() { exit; });

$PREFIX = $argv[1];

function csv2html($FILE) {
  $row=1;
  echo "<tbody>";
  if (($handle = fopen($FILE, "r")) !== FALSE) {
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      if($num>0) {
        echo "<tr>\n";    
        for($i=0;$i<$num;$i++) {
          echo "<td align=\"center\">".$data[$i]."</td>\n";
        }
        echo "</tr>\n";
      }
      $row++;
    }
    if($row==1) {
     echo "<h3>There are no rows!</h3>";
    }
  }
  else {
    echo "<h3>The file ".$FILE." was not found...</h3>\n";
  }
  echo "</tbody>";
}

function timesTable($FILE) {
  $row=1;

  if (($handle = fopen($FILE, "r")) !== FALSE) {

    $data = fgetcsv($handle, 1000, ",");
    $num = count($data);

    echo "<table>\n";
    echo "<thead>\n";    
    echo "<th align=\"center\" width=\"100px\">Job</th>\n";
    for($i=1;$i<=$num;$i++) { 
      echo "<th align=\"center\" width=\"150px\"> Worker ".$i."</th>\n";
    }
    echo "</thead>\n";

    echo "<tbody>";

    $job=1;
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      if($num>0) {
        echo "<tr>\n";    
        echo "<td align=\"center\">".$job."</td>\n";
        for($i=0;$i<$num;$i++) {
          $aux = $data[$i];

	  if($data[$i]==0)
	    $aux = "-";
	    
          echo "<td align=\"center\">".$aux."</td>\n";
        }
        echo "</tr>\n";
	$job++;
      }
      $row++;
    }

    if($row==1) {
     echo "<h3>There are no rows!</h3>";
    }

    echo "</tbody>\n";
    echo "</table>\n";

  }
  else {
    echo "<h3>The file ".$FILE." was not found...</h3>\n";
  }
}

function workerThroughput($FILE) {
  $row=1;

  if (($handle = fopen($FILE, "r")) !== FALSE) {
    echo "<table>\n";
    echo "<thead>\n";
    echo "<th width=\"150px\" align=\"center\">Worker</th>\n";
    echo "<th width=\"300px\" align=\"center\">Throughput (Jobs/sec)</th>\n";
    echo "</thead>\n";
    
    echo "<tbody>\n";
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      if($num>0) {
        echo "<tr>\n";    
        echo "<td align=\"center\">".$row."</td>\n";
        echo "<td align=\"center\">".$data[0]."</td>\n";
        echo "</tr>\n";
      }
      $row++;
    }
    
    echo "</tbody>\n";
    echo "</table>\n";
  }
  else {
    echo "<h3>The file ".$FILE." was not found...</h3>\n";
  } 
}

function workerCost($FILE) {
  $row=1;

  if (($handle = fopen($FILE, "r")) !== FALSE) {
    echo "<table>\n";
    echo "<thead>\n";
    echo "<th width=\"150px\" align=\"center\">Worker</th>\n";
    echo "<th width=\"200px\" align=\"center\">Ratio</th>\n";
    echo "</thead>\n";
    
    echo "<tbody>\n";
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      if($num>0) {
        echo "<tr>\n";    
        echo "<td align=\"center\">".$row."</td>\n";
        echo "<td align=\"center\">".$data[0]."</td>\n";
        echo "</tr>\n";
      }
      $row++;
    }
    
    echo "</tbody>\n";
    echo "</table>\n";
  }
  else {
    echo "<h3>The file ".$FILE." was not found...</h3>\n";
  } 
}
?>

<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8" />
    <title>SF3P Simulation Results</title>

    <link rel="stylesheet" href="http://code.jquery.com/ui/1.9.2/themes/base/jquery-ui.css" />

    <script src="http://jsgt.org/lib/jquery/plugin/csv2table/v002/js/jquery-1.7.2.min.js"></script>
    <script src="http://code.jquery.com/ui/1.9.2/jquery-ui.js"></script>	

    <script>    
      $(function() {
        $( "#tabs" ).tabs();
      });

      $(function() {
        $( "#metrics_accordion" ).accordion({heightStyle: "content", collapsible: true, active: false, alwaysOpen: false});
      });
    </script>
</head>
<body>

<h2>Simulation Summary:</h2>

<table>
  <tr>
    <td align="right"><b>Name:</b></td>
    <td>EDF</td>
  </tr>
  <tr>
    <td align="right"><b>Description:</b></td>
    <td>Simple EDF topology with 3 workers</td>
  </tr>
  <tr>
    <td align="right"><b>Duration:</b></td>
    <td>0h01m03s</td>
  </tr>
  <tr>
    <?php echo system("~/git/SF3P/scripts/bash/info.sh html"); ?>
  </tr>
</table>

<br>
 
<div id="tabs">
  <ul>
    <li><a href="#tabs-1">XML Input</a></li>
    <li><a href="#tabs-2">Raw Output</a></li>
    <li><a href="#tabs-3">Metrics</a></li>
    <li><a href="#tabs-4">Figures</a></li>
  </ul>
  <div id="tabs-1">
    <p>
      <textarea id="xml_input" rows="80" cols="100" border="0" >
	<?php echo "\n".file_get_contents("$PREFIX.xml")."\n"; ?>
      </textarea>
    </p>
  </div>
  <div id="tabs-2">
    
    <p>
      These are the pthread runtimes:
      <div id="runtimes_table">
	<table border=\"1\">
	  <thead>
	    <th width="150px">Thread Type</th>
	    <th width="150px">Thread ID</th>
	    <th width="150px">Runtime (us)</th>
          </thead>
          <?php csv2html($PREFIX."_runtimes.csv");  ?>
	</table>
      </div>
    </p>
    
    <p>
      These are the missed deadlines:
      <?php if (($handle = fopen($PREFIX."_missed_deadlines.csv", "r")) !== FALSE) { ?>
        <div id="missed_deadlines_table">
          <table border=\"1\">
	    <thead>
              <th width="150px">Thread ID</th>
              <th width="200px">Arrival Time (us)</th>
              <th width="200px">Deadline (us)</th>
              <th width="200px">Finish Time (us)</th>
            </thead>
            <?php csv2html($PREFIX."_missed_deadlines.csv");  ?>
          </table>
        </div>
      <?php }
      else { ?>
         <h3>There are no missed deadlines!</h3>
      <?php } ?>
    </p>
    
    <p>
      These are the traces:  	
      <div id="traces_table">
	      <table border=\"1\">
	        <thead>
	          <th width="200px">Timestamp (us)</th>
	          <th width="150px">Thread ID</th>
	          <th width="150px">Event</th>
                </thead>
                <?php csv2html($PREFIX."_traces.csv");  ?>
	      </table>
      </div>
    </p>
  </div> 
  <div id="tabs-3">
    <p>
      These are the metrics
      
      <div id="metrics_accordion">
	<h3>Execution Times</h3>
	<div>
       <p>
	    These are the execution times (ms):
            <?php timesTable($PREFIX."_exec_ms.csv");  ?>
       </p>
	</div>
	<h3>Response Times</h3>
	<div>
          <p>
	    These are the response times (ms):
            <?php timesTable($PREFIX."_resp_ms.csv");  ?>
          </p>
	</div>
	<h3>Missed Deadlines</h3>
	<div>
	  <p>
	    <?php 
	    if(file_exists($PREFIX."_deadline_metrics.csv")) { 
	    ?>
	    These are the missed deadlines:
            <div id="traces_table">
	      <table border=\"1\">
              <thead>
                <th width="200px">Worker</th>
                <th width="200px">% Missed</th>
                <th width="200px">Average Tardiness</th>
              </thead>
              <?php csv2html($PREFIX."_deadline_metrics.csv");  ?>
	      </table>
	    </div>		
	    <?php
	    } 
            else { 
	      echo "There are no missed deadlines!\n"; 
            } ?>
	  </p>
	</div>

	<h3>Throughput</h3>
	<div>
	  <p>
	    These are the worker costs:
	    <?php workerThroughput($PREFIX."_throughput.csv"); ?>
	  </p>
	</div>

	<h3>Utilization</h3>
	<div>
          <p>
	    This is the utilization:
	    <?php echo "\n".file_get_contents($PREFIX."_utilization.csv")."\n"; ?>
          </p>
	</div>
	<h3>Costs</h3>
	<div>
          <p>
	    These are the costs:

	    <table>
	    <thead>
	    <th width="200px" align="center"> Cost </th>
	    <th width="250px" align="center"> Value (us) </th>
	    </thead>
	    <tbody>
	    <tr>
		<td align="center">Resource Allocation</td>
		<td align="center"><?php echo "\n".file_get_contents($PREFIX."_alloc_cost_us.csv")."\n"; ?> us</td>
	    </tr>
	    <tr>
		<td align="center">Total System</td>
		<td align="center"><?php echo "\n".file_get_contents($PREFIX."_sys_cost_us.csv")."\n"; ?> us</td>
	    </tr>
	    </tbody>
	    </table>
          </p>
	  <p>
	    These are the worker costs:
	    <?php workerCost($PREFIX."_workerCost.csv"); ?>
	  </p>
	</div>
      </div>
      
    </p>
  </div>
  <div id="tabs-4">
    <p>
      Simulation Figure: <br>
      <img src="<?php echo $PREFIX."_data/".$PREFIX."_figure.svg"; ?>" alt="Simulation Figure" width="1000px"/>
    </p>
  </div>
</div>

</body>
</html>
