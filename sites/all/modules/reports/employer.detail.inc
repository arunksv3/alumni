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
              <li><a class="active" href="employerreports">No. of employers registered</a></li>
              <li><a href="employeradvertisejob">No. of employers advertise job</a></li>
              <li><a href="employerinterviewreports">No. of employers call for interview</a></li>
              <li><a href="employerpreferencesreports">Employers Preferences by</a></li>
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
     $form['filter']['wrapper']['job_country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'detail_list_state_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
       );
        // Wrapper for state dropdown list
      $form['filter']['wrapper']['job_country_prefered']['wrapper'] = array(
          '#prefix' => '<div id="state-list-wrapper">',
          '#suffix' => '</div>',
       );
       if ((isset($form_state['values']['job_country']) && $form_state['values']['job_country'] != ""))
       {
           $cid = $form_state['values']['job_country'];
           $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if(count($state_list) > 0){
            // Province dropdown list
            $form['filter']['wrapper']['job_country_prefered']['wrapper']['job_state'] = array(
                '#type' => 'select',
                '#title' => t('State'),
                '#options' => $state_list_arr,
            );
        }
       }
    $form['filter']['wrapper']['industry'] = array(
    '#type' => 'select',
    '#options' => array(''=>'select')+nature_bussiness_arr(),
    '#title' => 'Industry',
    );
    $form['filter']['wrapper']['status'] = array(
    '#type' => 'select',
    '#options' => array(''=>'select status',1=>'Active',0=>'Inactive'),
    '#title' => 'Status',
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
             array('data' => t('Email'), 'width' => '10%'),
             array('data' => t('Location'), 'width' => '10%'),
             array('data' => t('Industry'), 'width' => '48%'),
             array('data' => t('status'), 'width' => '5%'),
             array('data' => t('Register Date'), 'width' => '12%'),     
           );
    
       $results=db_select('employer_registration', 'r'); 
       $results->innerjoin('users', 'u', 'u.uid = r.uid');
       $results->fields('r',array('company_organization','nature_bussiness','country','state','created_date'));
       $results->fields('u',array('mail','status'));

       if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
        {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          $results->condition('r.created_date',$fromdate,'>=');
        }
       if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
        {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          $results->condition('r.created_date',$todate,'<=');
        }
       if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('r.uid',$form_state['values']['employer'],'=');
        }
       if(isset($form_state['values']['job_country'])&& $form_state['values']['job_country']!="")
        {
          $results->condition('r.country',$form_state['values']['job_country'],'=');
        } 
       if(isset($form_state['values']['job_state'])&& $form_state['values']['job_state']!="")
        {
          $results->condition('r.state',$form_state['values']['job_state'],'=');
        }  
       if(isset($form_state['values']['industry'])&& $form_state['values']['industry']!="")
        {
           $child_db_or = db_or();
           $child_db_or->condition('r.nature_bussiness',db_like($form_state['values']['industry']).'|%','LIKE');
           $child_db_or->condition('r.nature_bussiness',$form_state['values']['industry'],'=');
           $child_db_or->condition('r.nature_bussiness','%|' .db_like($form_state['values']['industry']).'|%','LIKE');
           $child_db_or->condition('r.nature_bussiness','%|' .db_like($form_state['values']['industry']),'LIKE');
           $results->condition($child_db_or);
        }  
        if(isset($form_state['values']['status'])&& $form_state['values']['status']!="")
        {
          $results->condition('u.status',$form_state['values']['status'],'=');
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
      $nature_bussiness_arr=nature_bussiness_arr();
      foreach($result as $res):
        $nature_bussiness_all="";
        $nature_bussiness=explode("|",$res->nature_bussiness);
         foreach($nature_bussiness as $nature_bussiness_ind):
          if($nature_bussiness_ind!="")
          $nature_bussiness_all.=$nature_bussiness_arr[$nature_bussiness_ind]."<br>";
         endforeach;   
       $created_date=date('d-m-Y',strtotime($res->created_date));
       if($res->status==1)
        $status='Active'; 
       else 
        $status='Inactive'; 
    
       
       $rows[] =array($res->company_organization,$res->mail,get_country_name($res->country)."<br>".get_state_name($res->state),$nature_bussiness_all,$status,$created_date);
       $pdf_records.="<tr><td>".$res->company_organization."</td><td>".$res->mail."</td><td>".get_country_name($res->country)."<br>".get_state_name($res->state)."</td><td>".$nature_bussiness_all."</td><td>".$status."</td><td>".$created_date."</td></tr>";
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
		<table width="85%" border="0" cellspacing="0" cellpadding="0" class="employer-detail">
		<tr>
		<th width="15%">Company Name</th>
		<th width="10%">Email</th>
		<th width="10%">Location</th>
		<th width="48%">Industry</th>
		<th width="5%">status</th>
        <th width="12%">Register Date</th></tr>'.$_SESSION['pdf_records'].'</table></div>
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