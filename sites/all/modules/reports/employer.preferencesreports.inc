<?php
/**
 * Student Summary Report Function.
 */
function ajax_qualification_detail_callback($form, $form_state) {
    return $form['filter']['wrapper']['highest']['qualification_level'];
}
function detail_report_select_ajax($form, $form_state) {
    return $form['filter']['wrapper'];
}
function detail_reports_submit($form, &$form_state) {  
   $form_state['rebuild'] = TRUE;
}
function detail_list_state_ajax($form, $form_state)
{
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper']['job_country_prefered']['wrapper'];   
}
function detail_reports($form, &$form_state) { 
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
        $("#download").click(function() {
         $("#download_pdf").val("pdf");
         document.getElementById("detail-reports").submit();
        });
        });', 'inline');
      drupal_add_js('function submit_change(){document.getElementById("download_pdf").value=""}','inline');
     $form = array();
     $form['filter']['title_links'] = array(
      '#type' => 'fieldset',
      '#title' => t('<ul class="employer-detail-tabs">
              <li><a href="employerreports">No. of employers registered</a></li>
              <li><a href="employeradvertisejob">No. of employers advertise job</a></li>
              <li><a href="employerinterviewreports">No. of employers call for interview</a></li>
              <li><a class="active" href="employerpreferencesreports">Employers Preferences by</a></li>
              </ul>'),
    );
    $form['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper" class="employer-detail-report">',
     '#suffix' => '</div>',
    );
    $form['filter']['download_pdf'] = array(
      '#type' => 'hidden',
      '#attributes' => array(
        'id' => 'download_pdf'
      )
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
  $form['filter']['wrapper']['employer'] = array(
    '#type' => 'select',
    '#options' => all_employers(),
    '#title' => 'Company Name',
    );
    $form['filter']['wrapper']['positionapplied'] = array(
    '#type' => 'select',
    '#options' => positionoffered(),
    '#title' => 'Position',
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
    );
   $form['filter']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a id="download">Download PDF</a></span>'), 
    );
       $header = array(
             array('data' => t('Company Name'), 'width' => '15%'), 
             array('data' => t('Job ID'), 'width' => '5%'),
             array('data' => t('Job Title'), 'width' => '35%'),
             array('data' => t('Hiring Position'), 'width' => '20%'),
             array('data' => t('Location'), 'width' => '10%'),
             array('data' => t('Job Posted Date'), 'width' => '15%'),         
           );
    
       $results=db_select('employer_registration', 'r'); 
       $results->innerjoin('employer_jobs', 'e', 'r.uid = e.uid');
       $results->fields('r',array('company_organization'));
       $results->fields('e',array('position_offered','jobid','job_title','date_time','job_country','job_state'));
      
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
        if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('r.uid',$form_state['values']['employer'],'=');
        }
        if(isset($form_state['values']['positionapplied'])&& $form_state['values']['positionapplied']!="")
        {
          $results->condition('e.position_offered',$form_state['values']['positionapplied'],'=');
        }
      $table_sort = $results->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

      $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('r.eid', 'DESC');
      if(isset($form_state['values']['download_pdf']) && $form_state['values']['download_pdf']=='pdf')
         $pager->limit(200000000); 
       else
         $pager->limit(10); //10 rows per page.

      $result = $pager->execute();
      $positionoffered=positionoffered();
      foreach($result as $res):  
        $created_date=date('d-m-Y',strtotime($res->date_time));      
        $rows[] =array($res->company_organization,$res->jobid,$res->job_title,$positionoffered[$res->position_offered],get_country_name($res->job_country)."<br>".get_state_name($res->job_state),$created_date);
        $pdf_records.="<tr><td>".$res->company_organization."</td><td>".$res->jobid."</td><td>".$res->job_title."</td><td>".$positionoffered[$res->position_offered]."</td><td>".get_country_name($res->job_country)."<br>".get_state_name($res->job_state)."</td><td>".$created_date."</td></tr>";
     endforeach; 
     $form['filter']['wrapper_table'] = array(
     '#prefix' => '<div class="wrapper_table">',
     '#suffix' => '</div>',
     );  
     $form['filter']['wrapper_table']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
        
   $form['filter']['wrapper_table']['pager'] = array('#markup' => theme('pager'));
   $_SESSION['pdf_records']=$pdf_records;
   if(isset($form_state['values']['download_pdf']) && $form_state['values']['download_pdf']=='pdf')
    {
         $markup = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<style>
table.employer-detail{border-collapse: collapse; border-color: #ccc; border-width: 1px; color: #333333; font-family: verdana,arial,sans-serif; font-size: 11px;}
table.employer-detail tr{background-color:#e8e8e8;}
table.employer-detail th{background-color:#dddddc; border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px;}
table.employer-detail td{border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px; background:#f7f7f5; font-size:11px;}
</style>
        </head>
        <body>
        <div style="text-align:center">
		<table width="100%" border="0" cellspacing="0" cellpadding="0" class="employer-detail">
		<tr>
		<th width="15%">Company Name</th>
		<th width="5%">Job ID</th>
		<th width="35%">Job Title</th>
		<th width="20%">Hiring Position</th>
		<th width="10%">Location</th>
        <th width="15%">Job Posted Date</th></tr>'.$_SESSION['pdf_records'].'</table></div>
        </body>
        </html>';
        require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
        $dompdf = new DOMPDF;
        $dompdf->load_html($markup);
        $dompdf->render();
        $dompdf->stream("job_detail_report.pdf");
    }
    return $form;
}

function all_years()
{
    $year_array['']='select year';  
   for($i=1991;$i<=2015;$i++)
   {
     $year_array[$i]=$i;  
   }
   return $year_array;
}
function all_employers()
{
  $employer_registration = db_select('employer_registration')
        ->fields('employer_registration',array('uid','company_organization')) 
        ->execute()->fetchAll();     
  $employer_registration_arr[''] = 'Select';
        foreach ($employer_registration as $value):
            $employer_registration_arr[$value->uid] =$value->company_organization;
        endforeach;
  return $employer_registration_arr;  
}