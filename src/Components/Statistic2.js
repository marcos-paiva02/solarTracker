import React, { useState, useEffect, math } from 'react'
import firebase from './firebase'
import { ref, onValue} from 'firebase/database'

import '../css/Statistic.css'

let totalVoltage = 0
let vef = 0
let totalCurrent = 0
let ief = 0

export default function Statistic() {     

    const voltageRef = ref(firebase, ('voltage2'))
    const currentRef = ref(firebase, ('current2'))
    
    useEffect(() => {
        onValue(voltageRef, (snapshot) => {
            const voltages = snapshot.val()
            const voltageList = []
            for (let id in voltages) {
                voltageList.push(voltages[id])
            }
            for (let i = 0; i < voltageList.length; i++) {
                totalVoltage = totalVoltage + parseFloat(voltageList[i])
            }
            vef = totalVoltage / voltageList.length
            totalVoltage = 0
        })

        onValue(currentRef, (snapshot) => {
            const currents = snapshot.val()
            const currentList = []
            for (let id in currents) {
                currentList.push(currents[id])
            }
            for (let i = 0; i < currentList.length; i++) {
                totalCurrent = totalCurrent + parseFloat(currentList[i])
            }
            ief = totalCurrent / currentList.length
            totalCurrent = 0
        })
    }, [])

    return (
        <div className='statistic-container'>
            <div className='statistic-title'>Medições médias sem rastreamento:</div>
            <div> <hr/> </div>
            <div className='statistic-data'>
                <div className='data-container'>
                    <div>Tensão:</div>
                    <div>{ vef.toFixed(2) }V</div>
                </div>
                <div className='data-container'>
                    <div>Corrente:</div>
                    <div>{ ief.toFixed(2) }A</div>
                </div>
                <div className='data-container'>
                    <div>Potência:</div>
                    <div>{ (vef*ief).toFixed(2) }W</div>
                </div>
            </div>               
        </div>
    )
}