import Vue from 'vue'
import QtBridge from '../bridge/qtbridge'

if(process.env.NODE_ENV === 'production')
    Vue.use(QtBridge)
