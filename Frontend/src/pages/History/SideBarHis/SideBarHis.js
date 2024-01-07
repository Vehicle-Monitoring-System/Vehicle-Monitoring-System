import classNames from 'classnames/bind';
import styles from './SideBarHis.module.scss';
import { useState, useEffect } from 'react';
import axios from 'axios';
import moment from 'moment';

const cx = classNames.bind(styles);

function SideBarHis({ setListPoly, setCenter }) {
    const [selectedDevice, setSelectedDevice] = useState([]);
    const [selectedTrip, setSelectedTrip] = useState([]);
    const [listDevice, setListDevice] = useState([]);
    const [listTrip, setListTrip] = useState([]);
    const [time, setTime] = useState(moment().format('YYYY-MM-DD'));

    useEffect(() => {
        const getDevice = async () => {
            const res = await axios.get(`${process.env.REACT_APP_BE_API}/Device`);
            setListDevice(res.data);
        };
        getDevice();
    }, []);

    useEffect(() => {
        const getTrip = async (deviceId, time) => {
            const res = await axios.get(`${process.env.REACT_APP_BE_API}/Gps`, { params: { deviceId, time } });
            setListTrip(res.data.data);
        };
        if (selectedDevice.length > 0) getTrip(selectedDevice[0], time);
    }, [selectedDevice, time]);

    useEffect(() => {
        const getPoly = async (deviceId, start, end) => {
            const res = await axios.get(`${process.env.REACT_APP_BE_API}/Gps/trip`, {
                params: { deviceId, start, end },
            });
            setListPoly(res.data);
            if (res.data.length > 0)
                setCenter({ lat: parseFloat(res.data[0].latitude), lng: parseFloat(res.data[0].longitude) });
        };
        if (selectedTrip.length > 0) getPoly(selectedDevice[0], selectedTrip[0].item1.time, selectedTrip[0].item2.time);
    }, [selectedTrip]);

    const handleChangeDevice = (item) => {
        const isSelected = selectedDevice.includes(item);

        setListTrip([]);

        if (isSelected) {
            setSelectedDevice([]);
        } else {
            setSelectedDevice([item]);
        }
    };

    const handleChangeTrip = (item) => {
        const isSelected = selectedTrip.includes(item);
        setListPoly([]);
        if (isSelected) {
            setSelectedTrip([]);
        } else {
            setSelectedTrip([item]);
        }
    };

    const checkTime = (time) => {
        const formatTime = moment(time).format('h:mm:ss a');
        return formatTime;
    };

    return (
        <div className={cx('wrapper')}>
            <h2>Lịch sử chuyến đi</h2>
            <div className={cx('line')}></div>
            <div className={cx('time')}>
                <h4>Chọn thời gian:</h4>
                <input
                    type="date"
                    id="dateInput"
                    name="dateInput"
                    value={time}
                    onChange={(e) => {
                        setTime(e.target.value);
                    }}
                />
            </div>
            <h4>Danh sách các xe:</h4>
            <div>
                {listDevice.map((item, index) => (
                    <div key={index} className={cx('device_item')}>
                        <input
                            type="checkbox"
                            id={item.deviceId}
                            checked={selectedDevice.includes(item.deviceId)}
                            onChange={() => handleChangeDevice(item.deviceId)}
                        />
                        <label htmlFor={item.deviceId}>{item.deviceLicensePlates}</label>
                    </div>
                ))}
            </div>

            <div>
                <h4>Danh sách chuyến đi:</h4>
                {listTrip.map((item, index) => (
                    <div key={index} className={cx('trip_item')}>
                        <input
                            type="checkbox"
                            id={index}
                            checked={selectedTrip.includes(item)}
                            onChange={() => handleChangeTrip(item)}
                        />
                        <label htmlFor={index}>
                            <div className={cx('item_time')}>{checkTime(item.item1.time)}</div> -
                            <div className={cx('item_time')}>{checkTime(item.item2.time)}</div>
                        </label>
                    </div>
                ))}
            </div>
        </div>
    );
}

export default SideBarHis;
