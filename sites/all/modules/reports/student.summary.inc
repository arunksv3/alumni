<?php
/**
 * Student Summary Report Function.
 */
function summary_reports_submit($form, &$form_state) {
 $form_state['rebuild'] = TRUE;
}
function summary_reports($form, &$form_state) { 
     $display="";
     drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
     drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function($) { 
        $("#from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#to").datepicker("option","minDate", selected)
        }
        });
        $("#to").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#from").datepicker("option","maxDate", selected)
        }
        });  
        });', 'inline');
    $form = array();
	 $form['filter']['wrapper'] = array(
     '#prefix' => '<div class="summary-report">',
     '#suffix' => '</div>',
    );
    $form['filter']['wrapper']['daterange'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date Range'),
    );
    $form['filter']['wrapper']['daterange']['from'] = array(
    '#type' => 'textfield', 
    '#id' => 'from', 
    '#title' => 'From',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['daterange']['to'] = array(
    '#type' => 'textfield', 
    '#id' => 'to', 
    '#title' => 'To',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['summary_report_radios'] = array(
    '#type' => 'radios',
    '#title' => t('<h2>Pick any one of the below to get the summary report</h2>'),
    '#options' => array(1=>'No.of students registered in the portal',2=>'No.of students applied for jobs',3=>'No.of students shortlisted for interview'
          ,4=>'No.of students employed',5=>'No.of students took psychometric test'),
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('summary_reports_submit'),
    );
    if (isset($form_state['values']['summary_report_radios'])&& $form_state['values']['summary_report_radios']!="") {
      $display_first='<span class="result">Result:</span><div class="print-down"><a href="javascript:window.print();">Print Report</a> | <a href="?pdf=pdf">Download as PDF</a></div>';
      if($form_state['values']['summary_report_radios']==1)
      {   
         $results = db_select('stdinfo','s');
         $results->fields('s');

         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = strtotime($from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s"));   
          $results->condition('s.created_date', $fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
           $to_input = explode('/',$form_state['values']['to']);
           $todate = strtotime($to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s"));      
           $results->condition('s.created_date',$todate,'<=');
         }

         $results_count=$results->execute();
         $count = $results_count->rowCount();
         
         if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
          $display="<div class='reslt-cont'>No.of students registered in the portal from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
         else 
          $display="<div class='reslt-cont'>No.of students registered in the portal till date is: $count</div>";
      
     
       } 
       else if($form_state['values']['summary_report_radios']==2)
       {
         $results = db_select('student_job_details','s');
                       $results->groupBy('s.uid');
                       $results->fields('s');

                    
         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          //$fromdate = date('Y-m-d',strtotime($form_state['values']['from']));
          $results->condition('s.created_date',$fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
           $to_input = explode('/',$form_state['values']['to']);
           $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          //$todate = date('Y-m-d',strtotime($form_state['values']['to']));
           $results->condition('s.created_date',$todate,'<=');
         }
           $results_count=$results->execute();
           $count = $results_count->rowCount();
          if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
           $display="<div class='reslt-cont'>No.of students applied for jobs from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
          else
           $display="<div class='reslt-cont'>No.of students applied for jobs till date is: $count</div>";
       
       }
       else if($form_state['values']['summary_report_radios']==3)
       {
         $results = db_select('student_job_details','s');
                       $results->groupBy('s.uid');
                       $results->fields('s');

         $results->condition('s.status',1,'=');     
         
         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          $results->condition('s.created_date',$fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          $results->condition('s.created_date',$todate,'<=');
         }
           $results_count=$results->execute();
           $count = $results_count->rowCount();
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
            $display="<div class='reslt-cont'>No.of students shortlisted for interview from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
           else 
            $display="<div class='reslt-cont'>No.of students shortlisted for interview till date is: $count</div>";
        
       }
      else if($form_state['values']['summary_report_radios']==4)
       {
         $results = db_select('student_job_details','s');
                       $results->groupBy('s.uid');
                       $results->fields('s');

         $results->condition('s.status',4,'=');     
         
         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          $results->condition('s.created_date',$fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          $results->condition('s.created_date',$todate,'<=');
         }
           $results_count=$results->execute();
           $count = $results_count->rowCount();
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
            $display="<div class='reslt-cont'>No.of students employed from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
           else 
            $display="<div class='reslt-cont'>No.of students employed till date is: $count</div>";
       }
       else if($form_state['values']['summary_report_radios']==5)
       {
         $results = db_select('psychometric_std_info`','t');
                       $results->fields('t');
 
         
         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          $results->condition('t.created_date',$fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          $results->condition('t.created_date',$todate,'<=');
         }
           $results_count=$results->execute();
           $count = $results_count->rowCount();
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
            $display="<div class='reslt-cont'>No.of students took psychometric test from date".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
           else 
            $display="<div class='reslt-cont'>No.of students took psychometric test till date is: $count</div>";
       }
      }
    if(isset($_GET['pdf']) && $_GET['pdf']=='pdf')
    {
         $markup = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        </head>
        <body>
        <div style="text-align:center">'. $_SESSION['display'].'</div>
        </body>
        </html>';
        require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
        $dompdf = new DOMPDF;
        $dompdf->load_html($markup);
        $dompdf->render();
        $dompdf->stream("students_summary.pdf");
    }
    $_SESSION['display'] = $display;
    $form['filter']['table'] = array(
      '#type' => 'fieldset', 
      '#title' =>$display_first.$display,

    );
    return $form;
}