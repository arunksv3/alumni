<?php

function events_news_news_list($form, &$form_state){
    global $base_url;
    global $user;
    $form = array(); 
    $header = array();
    $newsQuery = db_select('news')
        ->fields('news')
        ->condition('status',1,'=');
     $table_sort = $newsQuery->extend('TableSort') // Add table sort extender.
        ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
        ->orderBy('date', 'DESC')
        ->limit(10); // 10 rows per page.

    $newsResult = $pager->execute();
    $reportedArr = reported();
    $output = '<div class="form-item form-type-item"><ul class="viewall-ne-list">';
    $i=0;
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    foreach($newsResult as $row){
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $i++;
        if($i%2==0)
            $class = 'white';
        else 
            $class = 'grey';
       $id = encrypt($row->id,$decrypting_key);
        $output .='<li class="'.$class.'"> 
        <div class="view-ne-left">'.$image.'</div>
        <div class="view-ne-right">
        <span class="date-time">'.date('d M Y',strtotime($row->date)).'</span> 
        <h1><a href="'.$base_url.'/news/'.$id.'">'.$row->title.'</a></h1> 
        
        <h2><span>Reported By</span>: '.$reportedArr[$row->reported_by].'</h2>         
        <p>'.substr($row->description,0,200).'</p> 
         <a href="'.$base_url.'/news/'.$id.'">Read More</a>      
        </div></li> ';
    }
     $output .= '</ul></div>';
    /*$form['news'] = array(
        '#type' => 'item', 
        '#title' => '<h2>News</h2> '
    );*/
    
    $form['news']['list'] = array(
        '#type' => 'item',         
        '#markup' =>$output,  
     ); 
    $form['pager'] = array('#markup' => theme('pager')); 
     return $form;
    
    
}