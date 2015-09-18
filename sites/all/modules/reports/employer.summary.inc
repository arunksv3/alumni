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
    '#options' => array(1=>'No.of employers registered in the portal',2=>'No.of employers advertise job',3=>'No.of employers call for interview'
          ,4=>'Employers preferences by'),
    );
    $form['filter']['wrapper']['summary_industry'] = array(
    '#type' => 'select',
    '#options' =>nature_bussiness_arr() ,
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
         $role_id = 5;     
         $results = db_select('users_roles','r');
         $results->innerjoin('users', 'u', 'u.uid = r.uid');
         $results->fields('r');
         $results->condition('r.rid',$role_id,'=');
         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="") 
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = strtotime($from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s"));   
          $results->condition('u.created',$fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = strtotime($to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s"));      
          $results->condition('u.created',$todate,'<=');
         }
         $results_count=$results->execute();
         $count = $results_count->rowCount();
         if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
          $display="<div class='reslt-cont'>No.of employers registered in the portal from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
         else 
          $display="<div class='reslt-cont'>No.of employers registered in the portal till date is: $count</div>";
       } 
       else if($form_state['values']['summary_report_radios']==2)
       {
         $results = db_select('employer_jobs','e');
                       $results->groupBy('e.uid');
                       $results->fields('e');
                  
         if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");   
          $results->condition('e.date_time',$fromdate,'>=');
         }
         if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");      
          $results->condition('e.date_time',$todate,'<=');
         }
           $results_count=$results->execute();
           $count = $results_count->rowCount();
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
            $display="<div class='reslt-cont'>No.of employers advertise job from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
           else 
            $display="<div class='reslt-cont'>No.of employers advertise job till date is: $count</div>";
       }
       else if($form_state['values']['summary_report_radios']==3)
       {
         $results = db_select('student_interview_request','s');
                       $results->groupBy('s.employer_id');
                       $results->fields('s');
    
         
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
            $display="<div class='reslt-cont'>No.of employers call for interview from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
           else 
            $display="<div class='reslt-cont'>No.of employers call for interview till date is: $count</div>";
       }
      else if($form_state['values']['summary_report_radios']==4)
       {
         $results = db_select('employer_registration','e');
                       $results->fields('e'); 
         
         if (isset($form_state['values']['from'])&& $form_state['values']['from']!="")
         {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          $results->condition('e.created_date',$fromdate,'>=');
         }
         if (isset($form_state['values']['to'])&& $form_state['values']['to']!="")
         {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          $results->condition('e.created_date',$todate,'<=');
         }
         if (isset($form_state['values']['summary_industry'])&& $form_state['values']['summary_industry']!="") {
            $child_db_or = db_or();
            $child_db_or->condition('e.nature_bussiness',db_like($form_state['values']['summary_industry']).'|%','LIKE');
            $child_db_or->condition('e.nature_bussiness',$form_state['values']['summary_industry'],'=');
            $child_db_or->condition('e.nature_bussiness','%|' .db_like($form_state['values']['summary_industry']).'|%','LIKE');
            $child_db_or->condition('e.nature_bussiness','%|' .db_like($form_state['values']['summary_industry']),'LIKE');
            $results->condition($child_db_or);
         } 
         
           $results_count=$results->execute();
           $count = $results_count->rowCount();
           $nature_bussiness_arr=nature_bussiness_arr();
           
           if(isset($form_state['values']['from'])&& $form_state['values']['from']!="" && (isset($form_state['values']['to'])&& $form_state['values']['to']!=""))
            $display="<div class='reslt-cont'> Employers preferences by (".$nature_bussiness_arr[$form_state['values']['summary_industry']].") from date ".$form_state['values']['from']." to ".$form_state['values']['to']." is: $count</div>";
           else 
            $display="<div class='reslt-cont'>Employers preferences by (".$nature_bussiness_arr[$form_state['values']['summary_industry']].") till date is: $count</div>";
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